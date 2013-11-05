<?php
/**
 * @file
 * Contains default views on behalf of the theme editor module.
 */

/**
* Implements hook_views_default_views().
*/
function theme_editor_views_default_views() { 
  $view = new view;
  $view->name = 'theme_editor';
  $view->description = 'The default view for the Theme Editor file section and revisions tables';
  $view->tag = 'default';
  $view->base_table = 'theme_editor_files';
  $view->human_name = 'Theme Editor';
  $view->api_version = 3;
  $view->version = 7;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  
  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['use_ajax'] = TRUE;
  $handler->display->display_options['access']['type'] = 'none';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['exposed_form']['options']['submit_button'] = 'Filter';
  $handler->display->display_options['exposed_form']['options']['reset_button'] = TRUE;
  $handler->display->display_options['exposed_form']['options']['reset_button_label'] = 'Reset Filter';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'name' => 'name',
    'theme' => 'theme',
    'path' => 'path',
    'mime' => 'mime',
    'changed' => 'changed',
    'created' => 'created',
    'status' => 'status',
  );
  $handler->display->display_options['style_options']['default'] = 'path';
  $handler->display->display_options['style_options']['info'] = array(
    'name' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'theme' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'path' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'mime' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'changed' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'created' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'status' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  $handler->display->display_options['style_options']['empty_table'] = 0;
  /* Field: Files: Filename */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['external'] = 0;
  $handler->display->display_options['fields']['name']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['name']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_file'] = 1;
  /* Field: Files: Theme name */
  $handler->display->display_options['fields']['theme']['id'] = 'theme';
  $handler->display->display_options['fields']['theme']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['theme']['field'] = 'theme';
  $handler->display->display_options['fields']['theme']['label'] = 'Theme';
  $handler->display->display_options['fields']['theme']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['external'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['theme']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['theme']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['theme']['alter']['html'] = 0;
  $handler->display->display_options['fields']['theme']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['theme']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['theme']['hide_empty'] = 0;
  $handler->display->display_options['fields']['theme']['empty_zero'] = 0;
  $handler->display->display_options['fields']['theme']['link_to_theme'] = 1;
  /* Field: Files: Path */
  $handler->display->display_options['fields']['path']['id'] = 'path';
  $handler->display->display_options['fields']['path']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['path']['field'] = 'path';
  $handler->display->display_options['fields']['path']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['path']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['path']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['path']['alter']['external'] = 0;
  $handler->display->display_options['fields']['path']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['path']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['path']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['path']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['path']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['path']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['path']['alter']['html'] = 0;
  $handler->display->display_options['fields']['path']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['path']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['path']['hide_empty'] = 0;
  $handler->display->display_options['fields']['path']['empty_zero'] = 0;
  /* Field: Files: Mime type */
  $handler->display->display_options['fields']['mime']['id'] = 'mime';
  $handler->display->display_options['fields']['mime']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['mime']['field'] = 'mime';
  $handler->display->display_options['fields']['mime']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['external'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['mime']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['mime']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['mime']['alter']['html'] = 0;
  $handler->display->display_options['fields']['mime']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['mime']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['mime']['hide_empty'] = 0;
  $handler->display->display_options['fields']['mime']['empty_zero'] = 0;
  $handler->display->display_options['fields']['mime']['mime_format'] = 'machine';
  /* Field: Files: Updated date */
  $handler->display->display_options['fields']['changed']['id'] = 'changed';
  $handler->display->display_options['fields']['changed']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['changed']['field'] = 'changed';
  $handler->display->display_options['fields']['changed']['label'] = 'Last Changed';
  $handler->display->display_options['fields']['changed']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['external'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['changed']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['changed']['alter']['html'] = 0;
  $handler->display->display_options['fields']['changed']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['changed']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['changed']['hide_empty'] = 0;
  $handler->display->display_options['fields']['changed']['empty_zero'] = 0;
  $handler->display->display_options['fields']['changed']['date_format'] = 'short';
  /* Field: Files: Created date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Created';
  $handler->display->display_options['fields']['created']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['created']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['created']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['created']['alter']['external'] = 0;
  $handler->display->display_options['fields']['created']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['created']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['created']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['created']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['created']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['created']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['created']['alter']['html'] = 0;
  $handler->display->display_options['fields']['created']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['created']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['created']['hide_empty'] = 0;
  $handler->display->display_options['fields']['created']['empty_zero'] = 0;
  $handler->display->display_options['fields']['created']['date_format'] = 'short';
  /* Field: Files: File Exists */
  $handler->display->display_options['fields']['status']['id'] = 'status';
  $handler->display->display_options['fields']['status']['table'] = 'theme_editor_files';
  $handler->display->display_options['fields']['status']['field'] = 'status';
  $handler->display->display_options['fields']['status']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['status']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['status']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['status']['alter']['external'] = 0;
  $handler->display->display_options['fields']['status']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['status']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['status']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['status']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['status']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['status']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['status']['alter']['html'] = 0;
  $handler->display->display_options['fields']['status']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['status']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['status']['hide_empty'] = 0;
  $handler->display->display_options['fields']['status']['empty_zero'] = 0;
  $handler->display->display_options['fields']['status']['not'] = 0;
  /* Contextual filter: Theme Argument */
  $handler->display->display_options['arguments']['theme']['id'] = 'theme';
  $handler->display->display_options['arguments']['theme']['table'] = 'theme_editor_files';
  $handler->display->display_options['arguments']['theme']['field'] = 'theme';
  $handler->display->display_options['arguments']['theme']['ui_name'] = 'Theme Argument';
  $handler->display->display_options['arguments']['theme']['default_argument_type'] = 'fixed';
  $handler->display->display_options['arguments']['theme']['default_argument_skip_url'] = 0;
  $handler->display->display_options['arguments']['theme']['summary']['number_of_records'] = '0';
  $handler->display->display_options['arguments']['theme']['summary']['format'] = 'default_summary';
  $handler->display->display_options['arguments']['theme']['summary_options']['items_per_page'] = '25';
  $handler->display->display_options['arguments']['theme']['glossary'] = 0;
  $handler->display->display_options['arguments']['theme']['limit'] = '0';
  $handler->display->display_options['arguments']['theme']['transform_dash'] = 0;
  $handler->display->display_options['arguments']['theme']['break_phrase'] = 0;
  $handler->display->display_options['filter_groups']['groups'] = array(
    0 => 'AND',
  );
  /* Filter criterion: Files: Filename */
  $handler->display->display_options['filters']['name']['id'] = 'name';
  $handler->display->display_options['filters']['name']['table'] = 'theme_editor_files';
  $handler->display->display_options['filters']['name']['field'] = 'name';
  $handler->display->display_options['filters']['name']['operator'] = 'contains';
  $handler->display->display_options['filters']['name']['group'] = 0;
  $handler->display->display_options['filters']['name']['exposed'] = TRUE;
  $handler->display->display_options['filters']['name']['expose']['operator_id'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['label'] = 'Filename';
  $handler->display->display_options['filters']['name']['expose']['operator'] = 'name_op';
  $handler->display->display_options['filters']['name']['expose']['identifier'] = 'name';
  $handler->display->display_options['filters']['name']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['name']['expose']['multiple'] = FALSE;
  /* Filter criterion: Files: Mime type */
  $handler->display->display_options['filters']['mime']['id'] = 'mime';
  $handler->display->display_options['filters']['mime']['table'] = 'theme_editor_files';
  $handler->display->display_options['filters']['mime']['field'] = 'mime';
  $handler->display->display_options['filters']['mime']['operator'] = 'contains';
  $handler->display->display_options['filters']['mime']['group'] = 0;
  $handler->display->display_options['filters']['mime']['exposed'] = TRUE;
  $handler->display->display_options['filters']['mime']['expose']['operator_id'] = 'mime_op';
  $handler->display->display_options['filters']['mime']['expose']['label'] = 'Mime type';
  $handler->display->display_options['filters']['mime']['expose']['operator'] = 'mime_op';
  $handler->display->display_options['filters']['mime']['expose']['identifier'] = 'mime';
  $handler->display->display_options['filters']['mime']['expose']['required'] = FALSE;
  $handler->display->display_options['filters']['mime']['expose']['multiple'] = FALSE;
  $translatables['theme_editor'] = array(
    t('Master'),
    t('more'),
    t('Filter'),
    t('Reset Filter'),
    t('Sort by'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('Filename'),
    t('Theme'),
    t('Path'),
    t('Mime type'),
    t('Last Changed'),
    t('Created'),
    t('File Exists'),
    t('All'),
  );
  $views[$view->name] = $view;
  
  return $views;
}