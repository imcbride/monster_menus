<?php
// $Id: mm_constants.inc 5381 2011-05-03 17:05:32Z dwilga $

/**
 * @file
 * Various constants used by Monster Menus
 */

// Constants used in mm_get_node_info() and hook_mm_node_info()
define('MM_NODE_INFO_NO_REORDER', 'no_reorder'); // don't allow this node type to be reordered
define('MM_NODE_INFO_NO_RENDER',  'no_render');  // don't render this node type
define('MM_NODE_INFO_ADD_HIDDEN', 'add_hidden'); // hide the Add node link for this type

// Constants used in mm_content_get()
define('MM_GET_ARCHIVE',                '_arch');  // return archive status
define('MM_GET_FLAGS',                  '_flags'); // return flags
define('MM_GET_PARENTS',                '_par');   // return parents

// Constants used in mm_content_get_tree() and mm_content_get_query()
define('MM_GET_TREE_ADD_SELECT',        '_sel');   // add to list of selected columns
define('MM_GET_TREE_BIAS_ANON',         '_bias');  // assume user 0 can't read any groups
define('MM_GET_TREE_DEPTH',             '_depth'); // tree recursion depth
define('MM_GET_TREE_FAKE_READ_BINS',    '_read');  // pretend user has read on all bins (used internally)
define('MM_GET_TREE_FILTER_BINS',       '_bins');  // return recycle bins
define('MM_GET_TREE_FILTER_DOTS',       '_dots');  // return all entries with names starting with '.'
define('MM_GET_TREE_FILTER_GROUPS',     '_grps');  // get groups
define('MM_GET_TREE_FILTER_NORMAL',     '_norm');  // get entries not group or in /users
define('MM_GET_TREE_FILTER_USERS',      '_usrs');  // get entries in /users
define('MM_GET_TREE_INNER_FILTER',      '_inner'); // used internally
define('MM_GET_TREE_MMTID',             '_mmtid'); // tree ID to query
define('MM_GET_TREE_NODE',              '_node');  // node object to query permissions for
define('MM_GET_TREE_RETURN_BINS',       '_rbins'); // return list of parent recycle bins
define('MM_GET_TREE_RETURN_BLOCK',      '_rblk');  // return attributes from the mm_tree_block table
define('MM_GET_TREE_RETURN_FLAGS',      '_rflgs'); // return attributes from the mm_tree_flags table
define('MM_GET_TREE_RETURN_KID_COUNT',  '_rkids'); // return the number of children each entry has
define('MM_GET_TREE_RETURN_MTIME',      '_rmods'); // return the mtime and muid fields
define('MM_GET_TREE_RETURN_NODE_COUNT', '_rnode'); // include a "nodecount" field, containing the number of nodes using this entry
define('MM_GET_TREE_RETURN_PERMS',      '_rprms'); // return whether or not the user can perform an action
define('MM_GET_TREE_RETURN_TREE',       '_rtree'); // return attributes from the mm_tree table
define('MM_GET_TREE_SORT',              '_sort');  // sort results by weight, alpha, etc.; always TRUE when depth != 0
define('MM_GET_TREE_USER',              '_user');  // user object to test permissions against
define('MM_GET_TREE_WHERE',             '_where'); // add a WHERE clause to the outermost query

// Constants used only in mm_content_get_tree()
define('MM_GET_TREE_ADD_TO_CACHE',      '_cache'); // add results to the caches used by mm_content_get() and mm_content_get_parents()
define('MM_GET_TREE_BLOCK',             '_block'); // retrieve entries which appear in a particular block
define('MM_GET_TREE_FILTER_HIDDEN',     '_hide');  // get "hidden" entries
define('MM_GET_TREE_HERE',              '_here');  // list of tree IDs currently being viewed
define('MM_GET_TREE_ITERATOR',          '_iter');  // getTreeIterator (or subclass)
define('MM_GET_TREE_PRUNE_PARENTS',     '_ppar');  // prune parents depending upon max_parents
define('MM_GET_TREE_VIRTUAL',           '_virt');  // include virtual user list sub-entries

define('MM_GET_TREE_STATE_COLLAPSED',   (1<<0));
define('MM_GET_TREE_STATE_DENIED',      (1<<1));
define('MM_GET_TREE_STATE_EXPANDED',    (1<<2));
define('MM_GET_TREE_STATE_HERE',        (1<<3));
define('MM_GET_TREE_STATE_HIDDEN',      (1<<4));
define('MM_GET_TREE_STATE_LEAF',        (1<<5));
define('MM_GET_TREE_STATE_NOT_WORLD',   (1<<6));
define('MM_GET_TREE_STATE_RECYCLE',     (1<<7));

// Constants used in mm_content_copy()
define('MM_COPY_ALIAS',                 'alia');
define('MM_COPY_COMMENTS',              'comm');
define('MM_COPY_CONTENTS',              'cont');
define('MM_COPY_ITERATE_ALTER',         'itra');
define('MM_COPY_NAME',                  'name');
define('MM_COPY_NODE_PRESAVE_ALTER',    'noda');
define('MM_COPY_OWNER',                 'ownr');
define('MM_COPY_READABLE',              'read');
define('MM_COPY_RECUR',                 'recr');
define('MM_COPY_TREE',                  'tree');
define('MM_COPY_TREE_PRESAVE_ALTER',    'trea');
define('MM_COPY_TREE_SKIP_DUPS',        'tdup');

// Constants present in mm_tree.name
define('MM_ENTRY_NAME_DEFAULT_USER',    '.Default');
define('MM_ENTRY_NAME_DISABLED_USER',   '.Disabled');
define('MM_ENTRY_NAME_GROUPS',          '.Groups');
define('MM_ENTRY_NAME_RECYCLE',         '.Recycle');
define('MM_ENTRY_NAME_USERS',           '.Users');
define('MM_ENTRY_NAME_VIRTUAL_GROUP',   '.Virtual');

// The maximum number of sub-items per item in the tree is
// MM_CONTENT_BTOA_BASE ^ MM_CONTENT_BTOA_CHARS. If you might have more than
// this many /Users (or any other level of the tree) someday, increase
// MM_CONTENT_BTOA_CHARS and run mm_content_update_sort(). A larger
// MM_CONTENT_BTOA_BASE cannot be used, unless you are using a case-sensitive
// collation on the mmtree.sort_idx database column.
//
// The maximum nesting level of the tree is the length of mm_tree.sort_idx /
// MM_CONTENT_BTOA_CHARS. While you can increase this by altering the schema,
// you may find that MySQL starts to complain about there being too many tables
// in the JOIN in custom_url_rewrite_inbound(). MM_CONTENT_MYSQL_MAX_JOINS is
// used in monster_menus.install to ensure that this limit isn't exceeded.
define('MM_CONTENT_BTOA_START',      ord('!'));
define('MM_CONTENT_BTOA_BASE',       64);
define('MM_CONTENT_BTOA_CHARS',      4);
define('MM_CONTENT_MYSQL_MAX_JOINS', 61);

// Constants related to the virtual group "dirty" field
define('MM_VGROUP_DIRTY_NOT',       0); // not dirty
define('MM_VGROUP_DIRTY_NEXT_CRON', 1); // update during next hook_cron()
define('MM_VGROUP_DIRTY_FAILED',    2); // previously failed sanity check
define('MM_VGROUP_DIRTY_REDO',      3); // failed, but OK to regenerate
// If the count of users in a virtual group decreases by more than this ratio,
// return an error message and stop the vgroup regeneration. Set the matching
// record's "dirty" field in mm_vgroup_query to MM_VGROUP_DIRTY_REDO to ignore
// this condition and regenerate the group during the next run.
define('MM_VGROUP_COUNT_SANITY',    0.20);

// Constant used by mm_sitemap()
define('MM_SITEMAP_MAX_LEVEL', 6);

// Constants used by mm_ui.inc
define('MM_UI_MAX_USERS_IN_GROUP', 20); // max number of users to display (there can be more in the DB)
define('MM_UI_MAX_REORDER_ITEMS', 100); // max number of nodes/subpages to reorder

// Constant used by mm_query.inc
define('MM_QUERY_PAGE_SIZE', 50);

// Search feature: Set this value to TRUE to enable some debugging messages and
// to see the search query submitted via AJAX.
define('MMSR_debug', FALSE);

// Constants used by mm_browser.inc
define('MM_BROWSER_MODE_GALLERY', 'gal');
define('MM_BROWSER_MODE_GROUP',   'grp');
define('MM_BROWSER_MODE_MEDIA',   'med');
define('MM_BROWSER_MODE_PAGE',    'pag');
define('MM_BROWSER_MODE_RSS',     'rss');
define('MM_BROWSER_MODE_USER',    'usr');
define('MM_BROWSER_MODE_NODE',    'nod');
define('MM_BROWSER_MODE_MENU',    'men');

// Miscellaneous Constants

define('MM_HOME_MMTID_DEFAULT', 7);
define('MM_MENU_BID', 1); // block ID (bid) containing the page section title
define('MM_COMMENT_READABILITY_DEFAULT', 'comment readability default');  // permission name
