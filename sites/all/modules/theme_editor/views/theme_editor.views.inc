<?php
/**
 * @file Views integration for Theme Editor
 */

/**
 * Implements hook_views_data() 
 */
function theme_editor_views_data() {
  //Theme editor files base table info
  //Use a shortcut variable
  $dtef = &$data['theme_editor_files'];
  $dtefv = &$data['theme_editor_files_versions'];
  
  //Define a base group for the table
  $dtef['table']['group'] = t('Files');
  
  //Create the base table
  $dtef['table']['base'] = array(
    'field' => 'fid',
    'title' => t('Theme Editor Files'),
    'weight' => -10,
    'access query tag' => 'theme_editor_file_access',
    'defaults' => array(
      'field' => 'name',
    ),
  );
  
  // For other base tables, explain how we join
  $dtef['table']['join'] = array(
    // this explains how the 'files' table (named in the line above)
    // links toward the theme_editor_files_versions table.
    'theme_editor_files_versions' => array(
      'handler' => 'views_join', // this is actually optional
      'left_table' => 'theme_editor_files_versions', // Because this is a direct link it could be left out.
      'left_field' => 'fid',
      'field' => 'fid',
      // also supported:
      // 'type' => 'INNER',
      // 'extra' => array(array('field' => 'fieldname', 'value' => 'value', 'operator' => '='))
      //   Unfortunately, you can't specify other tables here, but you can construct
      //   alternative joins in the handlers that can do that.
      // 'table' => 'the actual name of this table in the database',
     ),
  );
  // ----------------------------------------------------------------
  // files table -- fields

  // fid
  $dtef['fid'] = array(
    'title' => t('Fid'),
    'help' => t('The File ID.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'handler' => 'views_handler_field_theme_editor_file',
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_theme_editor_fid',
      'name field' => 'name', // the field to display in the summary.
      'numeric' => TRUE,
      'validate type' => 'fid',
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  
  // name
  // This definition has more items in it than it needs to as an example.
  $dtef['name'] = array(
    'title' => t('Filename'), // The item it appears as on the UI,
    'help' => t('The file name.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'name', // the real field. This could be left out since it is the same.
      'group' => t('Files'), // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field_theme_editor_file',
      'click sortable' => TRUE,
      'link_to_file default' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  // Theme
  // This definition has more items in it than it needs to as an example.
  $dtef['theme'] = array(
    'title' => t('Theme name'), // The item it appears as on the UI,
    'help' => t('The Theme name.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'theme', // the real field. This could be left out since it is the same.
      'group' => t('Files'), // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field_theme_editor_theme',
      'click sortable' => TRUE,
      'link_to_theme default' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  // Path
  // This definition has more items in it than it needs to as an example.
  $dtef['path'] = array(
    'title' => t('Path'), // The item it appears as on the UI,
    'help' => t('The path with in the theme.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'field' => 'path', // the real field. This could be left out since it is the same.
      'group' => t('Files'), // The group it appears in on the UI. Could be left out.
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    // Information for accepting a title as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  
  // created field
  $dtef['created'] = array(
    'title' => t('Created date'), // The item it appears as on the UI,
    'help' => t('The date the created in Theme Editor.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  // changed field
  $dtef['changed'] = array(
    'title' => t('Updated date'), // The item it appears as on the UI,
    'help' => t('The date the file was last updated.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
  
  // Mime type
  $dtef['mime'] = array(
    'title' => t('Mime type'), // The item it appears as on the UI,
    'help' => t('The mime type (for example, "image/png", "text/html", "application/x-http-php", etc).'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_theme_editor_mime_type',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );
  
  // file status
  $dtef['status'] = array(
    'title' => t('File Exists'),
    'help' => t('Whether or not the file exists or is just a databse entry'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
      'output formats' => array(
        'published-notpublished' => array(t('Exists'), t('Database Only')),
      ),
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
      'label' => t('Exists'),
      'type' => 'yes-no',
      'use equal' => TRUE, // Use status = 1 instead of status <> 0 in WHERE statment
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // links to operate on the file

  $dtef['link_file'] = array(
    'field' => array(
      'title' => t('Link'),
      'help' => t('Provide a simple link to the file.'),
      'handler' => 'views_handler_field_theme_editor_file_link',
    ),
  );
  
  $dtef['revert_file'] = array(
    'field' => array(
      'title' => t('Revert link'),
      'help' => t('Provide a simple link to revert the file to a previous state.'),
      'handler' => 'views_handler_field_theme_editor_file_link_revert',
    ),
  );
  
  $dtef['delete_file'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the file.'),
      'handler' => 'views_handler_field_theme_editor_file_delete_link',
    ),
  );
  
  // Bogus fields for aliasing purposes.
  // Since the node arguments are already loaded we just use those. It's easier then recreating our own.

  $dtef['created_fulldate'] = array(
    'title' => t('Created date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $dtef['created_year_month'] = array(
    'title' => t('Created year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $dtef['created_year'] = array(
    'title' => t('Created year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $dtef['created_month'] = array(
    'title' => t('Created month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $dtef['created_day'] = array(
    'title' => t('Created day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $dtef['created_week'] = array(
    'title' => t('Created week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'created',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );

  $dtef['changed_fulldate'] = array(
    'title' => t('Updated date'),
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_fulldate',
    ),
  );

  $dtef['changed_year_month'] = array(
    'title' => t('Updated year + month'),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year_month',
    ),
  );

  $dtef['changed_year'] = array(
    'title' => t('Updated year'),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_year',
    ),
  );

  $dtef['changed_month'] = array(
    'title' => t('Updated month'),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_month',
    ),
  );

  $dtef['changed_day'] = array(
    'title' => t('Updated day'),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_day',
    ),
  );

  $dtef['changed_week'] = array(
    'title' => t('Updated week'),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => 'changed',
      'handler' => 'views_handler_argument_node_created_week',
    ),
  );
  
  // uid field
  $dtef['uid'] = array(
    'title' => t('Author'),
    'help' => t('Relate content to the user who created it.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'field' => 'uid',
      'label' => t('user'),
    ),
  );
  $dtef['uid_revision'] = array(
    'title' => t('User has a revision'),
    'help' => t('All files where a certain user has a revision'),
    'real field' => 'fid',
    'filter' => array(
      'handler' => 'views_handler_filter_theme_editor_file_uid_revision',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_theme_editor_file_uid_revision',
    ),
  );
  
  // FILE REVISIONS
  $dtefv['table']['group']  = t('File revision');
  
  // Advertise this table as a possible base table
  $dtefv['table']['base'] = array(
    'field' => 'vid',
    'title' => t('File revision'),
    'help' => t('File revision is a history of changes to files.'),
    'defaults' => array(
      'field' => 'title',
    ),
  );

  // For other base tables, explain how we join
  $dtefv['table']['join'] = array(
    // Directly links to node table.
    'theme_editor_files' => array(
      'left_field' => 'vid',
      'field' => 'vid',
    ),
  );
  // uid field for node revision
  $dtefv['uid'] = array(
    'title' => t('User'),
    'help' => t('Relate a content revision to the user who created the revision.'),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'users',
      'base field' => 'uid',
      'label' => t('revision user'),
    ),
  );

  // nid
  $dtefv['vid'] = array(
    'title' => t('Vid'),
    'help' => t('The revision ID of the file revision.'), // The help that appears on the UI,
    // Information for displaying the nid
    'field' => array(
      'click sortable' => TRUE,
    ),
    // Information for accepting a nid as an argument
    'argument' => array(
      'handler' => 'views_handler_argument_node_vid',
      'click sortable' => TRUE,
      'numeric' => TRUE,
    ),
    // Information for accepting a nid as a filter
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    // Information for sorting on a nid.
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'handler' => 'views_handler_relationship',
      'base' => 'theme_editor_files',
      'base field' => 'fid',
      'title' => t('File'),
      'label' => t('Get the actual file from a file revision.'),
    ),
  );

  // log field
  $dtefv['log'] = array(
    'title' => t('Log message'), // The item it appears as on the UI,
    'help' => t('The log message entered when the revision was created.'), // The help that appears on the UI,
     // Information for displaying a title as a field
    'field' => array(
      'handler' => 'views_handler_field_xss',
     ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );

  // revision timestamp
  // changed field
  $dtefv['timestamp'] = array(
    'title' => t('Created date'), // The item it appears as on the UI,
    'help' => t('The date the file revision was created.'), // The help that appears on the UI,
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );

  $dtefv['revert_revision'] = array(
    'field' => array(
      'title' => t('Revert link'),
      'help' => t('Provide a simple link to revert to the revision.'),
      'handler' => 'views_handler_field_node_revision_link_revert',
    ),
  );

  $dtefv['delete_revision'] = array(
    'field' => array(
      'title' => t('Delete link'),
      'help' => t('Provide a simple link to delete the file revision.'),
      'handler' => 'views_handler_field_node_revision_link_delete',
    ),
  );
  
  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function theme_editor_views_data_alter(&$data) { 
  $data['users']['table']['join']['theme_editor_files'] = array(
      'left_table' => 'theme_editor_files',
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'INNER', // all files have an author.
    );
    $data['users']['table']['join']['theme_editor_files_versions'] = array(
      'left_table' => 'theme_editor_files_version',
      'left_field' => 'uid',
      'field' => 'uid',
      'type' => 'INNER', // all files have an author.
    );
}