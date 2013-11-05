<?php
/**
 * @file The various functions needed for creating the Theme Editor editing enviroment.
 */

/**
 * Generates the Theme Editors Editing enviroment
 * 
 * @param $theme
 *   A Theme Object as built by list_themes()
 * 
 * @see list_themes();
 */
function theme_editor_editor($theme, $file = NULL) {
  if (!theme_editor_req_check()) {
    return '';
  }
  //Adds any of the files we need.
  _theme_editor_add_files();
  
  $build = db_query('SELECT name FROM {theme_editor_themes} WHERE name = :name', array(':name' => $theme->name))->fetch();
  if (!$build) {
    _theme_editor_build_theme($theme);
  }
  if (empty($file)) {
    $query = db_select('theme_editor_files', 'f')
     ->range(0,1);
    $query->condition('f.theme', $theme->name);
    $query->condition('f.name', $theme->name . '.info');
    $query->fields('f');
    $file = $query->execute()->fetch();
    drupal_goto(_tep($file, 'file'));
  }
  if (!empty($file)) {
    drupal_set_title($file->name . ' - ' . $theme->info['name'] . ' Editor');
    $out = drupal_get_form('theme_editor_editor_form', $theme, $file);
  }
  else {
    $out = '';
    drupal_set_message(t('There was a problem fetching the theme or file. Please go back and try this themes editor again.'), 'error');
  }
  return $out;
}

/**
 * Menu Callback that displays the list of themes available to edit
 */
function theme_editor_list() {
  if (!theme_editor_req_check()) {
    return '';
  }
  $themes = list_themes();
  foreach ($themes as &$theme) {
    if (!empty($theme->info['hidden'])) {
      continue;
    }
    $link = l($theme->info['name'] . ' Editor', _tep($theme, 'theme'));
    $desc = '<div class="description">' . t($theme->info['description']) . '</div>';
    $item = $link . $desc;
    if(!empty($theme->base_theme)) {
      $items[$theme->base_theme]['children'][$theme->name] = $item;
    }
    else{
      $items[$theme->name]['data'] = $item;
    }
  }
  $vars = array(
    'items' => $items,
    'title' => t('Editors Avaialble'),
    'type' => 'ul',
  );
  return theme('item_list', $vars);
}

/**
 * Form callback for the Theme Editor Editor form.
 */
function theme_editor_editor_form($form, &$form_state, $theme, $file) {
  $global = theme_editor_get_theme_settings($theme->name);
  $form['#theme_file'] = $file;
  $form['#theme_theme'] = $theme;
  $form['file_info']['#markup'] = theme('theme_editor_file_info', array('file' => $file));
  $name = empty($file->path) ? $file->name : $file->path . '/' . $file->name;
  $form['editor'] = array(
    '#type' => 'textarea',
    '#title' => $name,
    '#rows' => 40,
    '#default_value' => $file->data
  );
  $form['buttons']['save'] = array(
    '#type' => 'submit',
    '#value' => 'Save',
  );
  $form['buttons']['save_not_activate'] = array(
      '#type' => 'submit',
      '#value' => 'Save & Leave Disabled',
      '#access' => FALSE,
    );
  if (!$theme->info['theme_editor']['status']) {
    $form['buttons']['save']['#value'] = 'Save & Enable Changes';
    $form['buttons']['save_not_activate']['#access'] = TRUE;
  }
  
  $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
  );
  $form['buttons']['cancel'] = array(
    '#markup' => l('Cancel Changes', $_GET['q']),
  );
  if ($theme->info['theme_editor']['status']) {
    $form['buttons']['disable'] = array(
      '#markup' => ' | ' . l('Disable Theme Changes', 'admin/appearance/editor/toggle/' . $theme->name . '/Enable',
        array('query' => array(
            'destination' => $_GET['q'],
          ),
        )
      ),
    );
  }
  $form['options_tabs'] = array(
    '#type' => 'vertical_tabs'
  );
  $form['options_tabs']['options'] = array(
    '#type' => 'fieldset',
    '#title' => 'File Options',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
      'class' => array('theme-editor-editor-options'),
    ),
  );
  $form['options_tabs']['options']['save_as'] = array(
    '#type' => 'select',
    '#default_value' => array($global['save_as']),
    '#title' => 'Save File As',
    '#options' => array(
      'filesystem' => t('File with DB Backup'),
      'database' => t('Database entry only'),
    ),
    '#description' => t('How to save the file: Either to a file or as a DB backup only.'),
    '#attributes' => array(
      'class' => array('theme-editor-editor-options-save-as'),
    ),
  );
  $form['options_tabs']['versions'] = array(
    '#type' => 'fieldset',
    '#title' => 'File Backup',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
      'class' => array('theme-editor-editor-versions'),
    ),
  );
  $form['options_tabs']['versions']['revision'] = array(
    '#type' => 'checkbox',
    '#default_value' => $global['revision'],
    '#title' => 'Create Revision',
    '#states' => array(
      'checked' => array(
        'textarea[name="log"]' => array('empty' => FALSE),
      ),
    ),
  );
  $form['options_tabs']['versions']['log'] = array(
    '#type' => 'textarea',
    '#title' => t('Log Message'),
  );
  
  $form['options_tabs']['actions'] = array(
    '#type' => 'fieldset',
    '#title' => 'File and Theme Actions',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
      'class' => array('theme-editor-editor-actions'),
    ),
    '#description' => t('Actions that can be performed on the Theme and File'),
  );
  
  $actions = theme_editor_get_file_actions();
  $actions_list = array();
  foreach ($actions as $name => $action) {
    if (empty($action['hide'])) {
      $href = 'admin/appearance/editor/theme/' . $theme->name . '/file_actions/' . $name;
      if ($action['scope'] == 'file') {
        $href .= '/' . $file->fid;
      }
      $actions_list[$action['scope']][] = l($action['title'], $href) . '<div class="description">' . $action['description'] . '</div>';
    }
  }
  
  $action_links = '';
  foreach($actions_list as $name => $links) {
    $action_links .= theme('item_list', array('items' => $links, 'title' => ucfirst($name)));
  }
  
  $form['options_tabs']['actions']['links']['#markup'] = $action_links;
  
  
  $view = views_get_view('theme_editor');
  
  $form['files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Theme Files'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#attributes' => array(
      'class' => array('theme-editor-editor-theme-files'),
    ),
  );
  $form['files']['view'] = array(
    '#markup' => $view->preview('default', array($theme->name)),
  );
  
  return $form;
}

/**
 * Form submit for Editor form
 */
function theme_editor_editor_form_submit(&$form, &$form_state) {
  global $user;
  $save = $form_state['values']['save'];
  $save_not_activate = $form_state['values']['save_not_activate'];
  $delete = $form_state['values']['delete'];
  $is_new = NULL;
  $theme = $form['#theme_theme'];
  $file = $form['#theme_file'];
  $flags['revision'] = $form_state['values']['revision'];
  if ($form_state['values']['save_as'] != 'filesystem') {
    dpm($form_state);
    $flags['filesystem'] = FALSE;
  }
  //If the file contents has changed or if the file is not in the filesystem and has been set to be saved there
  //Then we need to resave the data to the DB and FS.
  if ($file->data != $form_state['values']['editor'] || ($file->status == 0 && $form_state['values']['save_as'] == 'filesystem')) {
    $file->data = $form_state['values']['editor'];
    $file->log = $form_state['values']['log'];
  }
  else {
    //Don't do anything as the data has not changed
    drupal_set_message(t('The file data has not changed so no action was taken.'));
    return;
  }
  switch ($form_state['values']['op']) {
    case $delete:
      $form_state['redirect'] = 'admin/appearance/editor/theme/' . $file->theme . '/file_actions/delete_file/' . $file->fid;
      break;
    case $save:
      theme_editor_file_process($file, $theme, $flags);
      if ($theme->info['theme_editor']['status'] == 0) {
        $form_state['redirect'] = array('admin/appearance/editor/toggle/' . $theme->name . '/Enable',
          array('query' => array(
              'destination' => $_GET['q'],
            ),
          )
        );
      }
      break;
    case $save_not_activate:
      theme_editor_file_process($file, $theme, $flags);
      break;
  }
  drupal_set_message(t('File was processed and saved.'));
}

/**
 * Form callback for editing/changing Images in themes
 */
function theme_editor_editor_images_form($form, $form_state) {
  
}