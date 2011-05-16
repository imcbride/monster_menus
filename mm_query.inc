<?php
// $Id: mm_query.inc 5187 2011-03-21 18:49:44Z dan $

/**
 * @file
 * Cached data retrieval functions
 */

/**
 * Classes for mm_query
 *
 * The mmqCache class contains the SQL statement handle and cached data. Only
 * one instance of this class is created per unique SQL statement.
 *
 * The mmqIterator class maintains a pointer ($count) into the cached data. A
 * new instance of this class is instantiated whenever mm_query is called. In
 * this way, multiple callers can read different parts of the data cache for a
 * given statement simultaneously.
 */

require_once('mm_constants.inc');

class mmqIterator {
  private $count, $cache;

  public function __construct($query, array $args, $mmtid) {
    if (!is_numeric($mmtid))
      throw new InvalidArgumentException('$mmtid must be an integer.');
    
    global $_mmq_cache;

    $cacheKey = $query.serialize($args);

    $this->count = 0;
    if (isset($_mmq_cache[$cacheKey])) {
      $this->cache = $_mmq_cache[$cacheKey];
      return $_mmq_cache[$cacheKey]->reset($query);
    }

    return $this->cache = $_mmq_cache[$cacheKey] = new mmqCache($query, $args, $mmtid);
  }

  public function next() {
    return $this->cache->next($this->count);
  }

  public function back() {
    $this->cache->back($this->count);
  }
}

class mmqCache {
  public $mmtid;
  private $stmt, $data, $error, $overflow, $query, $prev, $prev_buffer, $args;

  public function __construct($query, array $args, $mmtid) {
    $this->overflow = FALSE;
    $this->data = array();
    $this->mmtid = $mmtid;
    $this->args = $args;
    $this->all_cols = !strncasecmp($query, 'SELECT * FROM {mm_tree}', 23);
    if ($mmtid < 0) {
      $this->stmt = NULL;
    } else {
      $this->query = $query;
      $this->stmt = db_query($this->query, $this->args);
    }
    if (!$this->stmt) $this->error = TRUE;
  }

  public function reset($query) {
    if ($this->overflow) $this->query = $query;
    $this->prev = FALSE;
    $this->prev_buffer = NULL;

    return $this;
  }

  public function back($count) {
    if (!$count) {
      $this->error = TRUE;
    }
    else {
      $this->prev = TRUE;
    }
  }

  public function next(&$count) {
    if ($this->error) return NULL;

    if ($this->prev) {
      $this->prev = FALSE;
      // intentionally don't clear prev_buffer, so that if another back()
      // follows, it will work
      return $this->prev_buffer;
    }
    else if ($count < count($this->data)) {
      return $this->prev_buffer = clone $this->data[$count++];
    }
    else if ($this->stmt) {
      $count++;

      if ($r = $this->stmt->fetch()) {
        if ($count > MM_QUERY_PAGE_SIZE) {    // don't waste too much memory
          $this->overflow = TRUE;
          return $this->prev_buffer = $r;
        }

        $this->data[] = $r;

        if ($this->all_cols) {
          global $_mmtbt_cache;
          $_mmtbt_cache[$r->mmtid] =& $this->data[count($this->data) - 1];
        }

        return $this->prev_buffer = $r;
      }

      $this->stmt = NULL;
    }
    else if ($this->overflow && !($count % MM_QUERY_PAGE_SIZE)) {
      if (stripos($this->query, ' limit ') > 0) {
        $this->stmt = db_query($this->query, $this->args);

        if (!$this->stmt) $this->error = TRUE;
        else {
          $c = 0;
          $count++;

          while ($c++ < $count) // skip past cached results, saving last
            if (!($r = $this->stmt->fetch())) break;

          if (!$r) $this->stmt = NULL;

          return $this->prev_buffer = $r;
        }
      }
      else {
        $this->stmt = db_query_range($this->query, $count, 0x7fffffff, $this->args);

        if (!$this->stmt) $this->error = TRUE;
        else {
          $count++;
          $r = $this->stmt->fetch();
          if (!$r) $this->stmt = NULL;

          return $this->prev_buffer = $r;
        }
      }
    }

    return $this->prev_buffer = NULL;
  }
}

/**
 * Start a new DB query, caching the results, or return an object to a
 * previously cached set of values.
 *
 * @param $query
 *   The SQL query
 * @param optional array $args
 *   An array of arguments for the query in PDO placeholder syntax.
 * @param optional $mmtid
 *   The optional tree ID of the entry being retrieved. This allows
 *   mm_content_clear_caches() to flush the cache only when needed. This
 *   parameter can be completely omitted, in which case the query's cache will
 *   be flushed whenever mm_content_clear_caches() is called.
 * @return
 *   If this function returns NULL, an error occurred. Otherwise, to obtain each
 *   row, call the returned value's next() function. If next() returns NULL, the
 *   end of the data set has been reached.
 */
function mm_query($query, array $args = array(), $mmtid = 0) {
  if (!is_numeric($mmtid))
    throw new InvalidArgumentException('$mmtid must be an integer.');
  
  return new mmqIterator($query, $args, $mmtid);
}

/**
 * Start a new DB query, caching the results, or return an object to a
 * previously cached set of values. This version allows a subset of the results
 * to be returned, using the LIMIT clause.
 *
 * @param $query
 *   The SQL query
 * @param $from
 *   The first result row to return.
 * @param $count
 *   The maximum number of result rows to return.
 * @param optional array $args
 *   An array of arguments for the query in PDO placeholder syntax.
 * @param optional $mmtid
 *   The optional tree ID of the entry being retrieved. This allows
 *   mm_content_clear_caches() to flush the cache only when needed. This
 *   parameter can be completely omitted, in which case the query's cache will
 *   be flushed whenever mm_content_clear_caches() is called.
 * @return
 *   If this function returns NULL, an error occurred. Otherwise, to obtain each
 *   row, call the returned value's next() function. If next() returns NULL, the
 *   end of the data set has been reached.
 */
function mm_query_range($query, $from, $count, array $args = array(), $mmtid = 0) {
  if (!is_int($from))
    throw new InvalidArgumentException(t('$from must be an integer, %val given.'. array('%val' => $from)));
  if (!is_int($count))
    throw new InvalidArgumentException(t('$count must be an integer, %val given.'. array('%val' => $count)));
  if (!is_numeric($mmtid))
    throw new InvalidArgumentException('$mmtid must be an integer.');
  
  $query .= ' LIMIT :mm_query_range_from, :mm_query_range_count';
  $args[':mm_query_range_from'] = $from;
  $args[':mm_query_range_count'] = $count;
  
  return new mmqIterator($query, $args, $mmtid);
}

/**
 * Start a new DB query, cache and return one result.
 *
 * @param $query
 *   The SQL query
 * @param optional array $args
 *   An array of arguments for the query in PDO placeholder syntax.
 * @param optional $mmtid
 *   The optional tree ID of the entry being retrieved. This allows
 *   mm_content_clear_caches() to flush the cache only when needed. This
 *   parameter can be completely omitted, in which case the query's cache will
 *   be flushed whenever mm_content_clear_caches() is called.
 * @return
 *   The single result field, or NULL on error
 */
function mm_query_result($query, array $args = array(), $mmtid = 0) {
  if (!is_numeric($mmtid))
    throw new InvalidArgumentException('$mmtid must be an integer.');
  
  if (!($q = new mmqIterator($query, $args, $mmtid)) || !($row = $q->next()))
    return NULL;

  $row = (array)$row;
  return array_shift($row);
}


/**
 * Start a new DB query, cache and return one row as an object.
 *
 * @param $query
 *   The SQL query
 * @param optional array $args
 *   An array of arguments for the query in PDO placeholder syntax.
 * @param optional $mmtid
 *   The optional tree ID of the entry being retrieved. This allows
 *   mm_content_clear_caches() to flush the cache only when needed. This
 *   parameter can be completely omitted, in which case the query's cache will
 *   be flushed whenever mm_content_clear_caches() is called.
 * @return
 *   The single result field, or NULL on error
 */
function mm_query_result_object($query, array $args = array(), $mmtid = 0) {
  if (!is_numeric($mmtid))
    throw new InvalidArgumentException('$mmtid must be an integer.');
  
  if (!($q = new mmqIterator($query, $args, $mmtid)) || !($row = $q->next()))
    return NULL;

  return $row;
}
