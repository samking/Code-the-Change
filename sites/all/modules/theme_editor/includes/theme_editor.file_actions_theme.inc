<?php
/**
 * @file Callback and form functions for hook_theme_editor_file_actions_info actions in the theme scope
 */

/**
 * Callback funciton for the add_file Action
 * 
 * @param $theme
 *   The Theme object
 * 
 * @return
 *   A populated Drupal Form.
 */
function theme_editor_file_actions_add($theme) {
  return drupal_get_form('theme_editor_file_actions_add_form', $theme);
}

/**
 * Form API function for add-file action
 * 
 * @see theme_editor_file_actions_add()
 */
function theme_editor_file_actions_add_form($form, &$form_state, $theme) {
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );
  $info = '<h3>' . t('Add a file to the !theme theme', array('!theme' => $theme->info['name'])) . '</h3>';
  $form['info']['#markup'] = $info;
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Filename'),
    '#required' => TRUE,
    '#description' => t('The filename, including extenstension, of the file to be added. Example: example.tpl.php, example.css, or example.js'),
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('File Path'),
    '#description' => t('The path within the theme where the file should be created. Example: css/exapmle, templates/page, or leave blank for the base directory'),
  );
  $upload_link = l('Upload File', 'admin/appearance/editor/theme/' . $theme->name . '/file_actions/upload_file');
  $form['data'] = array(
    '#type' => 'textarea',
    '#title' => t('Initial File Contents'),
    '#description' => t('The initial contents the file should be created with. If you already have a complete file it may be easier to use the !link action.',
      array('!link' => $upload_link)
    ),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Add File',
  );
  return $form;
}

/**
 * Form Validation handler
 * 
 * @see theme_editor_file_actions_add_form()
 */
function theme_editor_file_actions_add_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  
  $query = db_select('theme_editor_files', 'f');
  $query->fields('f', array('fid'));
  $query->condition('name', $values['name']);
  $query->condition('path', $values['path']);
  $query->condition('theme', $values['theme']->name);
  
  $fid = $query->execute()->fetchField(0);
  
  if (is_numeric($fid)) {
    form_set_error('name', t('A file with that name and path already exists. Please choose a new name or path.'));
    $form['path']['#attributes']['class'][] = 'error';
  }
  else {
    //The first thing we do is build our file object
    $remove = array('op', 'save', 'form_build_id', 'form_token', 'form_id');
    foreach($remove as $key) {
    unset($values[$key]);
    }
    
    $theme = $values['theme'];
    $values['theme'] = $theme->name;
    $file = (object)$values;
    theme_editor_file_process($file, $theme);
    
    if ($file->fid) {
      drupal_set_message(t('!filename added to !theme', array('!filename' => $file->name, '!theme' => $theme->info['name'])));
      $form_state['file'] = $file;
    }
    else {
      form_set_error('name', t('There was a problem adding your file to the theme. Please try again.'));
      $form['path']['#attributes']['class'][] = 'error';
    }
  }
}

/**
 * Form Submit handler
 * 
 * @see theme_editor_file_actions_add_form()
 */
function theme_editor_file_actions_add_form_submit($form, &$form_state) {  
  $file = $form_state['file'];
  
  $form_state['redirect'] = _tep($file, 'file');  
}

/**
 * Callback funciton for the upload_file Action
 * 
 * @param $theme
 *   The Theme object
 * 
 * @return
 *   A populated Drupal Form.
 */
function theme_editor_file_actions_upload($theme) {
  return drupal_get_form('theme_editor_file_actions_upload_form', $theme);
}

/**
 * Form API function for upload-file action
 * 
 * @see theme_editor_file_actions_upload()
 */
function theme_editor_file_actions_upload_form($form, &$form_state, $theme) {
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );
  $info = '<h3>' . t('Upload a file to the !theme theme', array('!theme' => $theme->info['name'])) . '</h3>';
  $form['info']['#markup'] = $info;
  $add_link = l('Add File', 'admin/appearance/editor/theme/' . $theme->name . '/file_actions/add_file');
  $form['file'] = array(
    '#type' => 'file',
    '#title' => t('File to upload'),
    '#description' => t('The file to upload to the theme. If you would like to create a blank or simple text file it may be easier to use the !link action.',
      array('!link' => $add_link)
    ),
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('New Filename'),
    '#description' => t('The filename, including extenstension, to rename the uploaded file to. Leave blank to keep the original filename. Example: example.tpl.php, example.css, or example.js'),
  );
  $form['path'] = array(
    '#type' => 'textfield',
    '#title' => t('File Path'),
    '#description' => t('The path within the theme where the file should be created. Example: css/exapmle, templates/page, or leave blank for the base directory'),
  );
  $form['save'] = array(
    '#type' => 'submit',
    '#value' => 'Upload File',
  );
  //Make the form accept files
  $form['#attributes'] = array('enctype' => "multipart/form-data");
  return $form;
}

/**
 * Form Validation handler
 * 
 * @see theme_editor_file_actions_upload_form()
 */
function theme_editor_file_actions_upload_form_validate(&$form, &$form_state) {
  $values = $form_state['values'];
  
  $file->name = !$values['name'] ? $_FILES['files']['name']['file'] : $values['name'];
  $file->filename = $_FILES['files']['tmp_name']['file'];
  
  $query = db_select('theme_editor_files', 'f');
  $query->fields('f', array('fid'));
  $query->condition('name', $file->name);
  $query->condition('path', $values['path']);
  $query->condition('theme', $values['theme']->name);
  
  $fid = $query->execute()->fetchField(0);
  
  if (is_numeric($fid)) {
    form_set_error('name', t('A file with that name and path already exists. Please choose a new name or path.'));
    $form['path']['#attributes']['class'][] = 'error';
    $form['file']['#attributes']['class'][] = 'error';
    return;
  }
  
  $file->mime = $_FILES['files']['type']['file'];
  $file->path = $values['path'];
  $theme = $values['theme'];
  theme_editor_file_process($file, $theme);
  
  if ($file->fid) {
    drupal_set_message(t('!filename uploaded to !theme', array('!filename' => $file->name, '!theme' => $theme->info['name'])));
    $form_state['file'] = $file;
  }
  else {
    form_set_error('name', t('There was a problem uploading the file to your theme. Please try again.'));
    $form['path']['#attributes']['class'][] = 'error';
    $form['file']['#attributes']['class'][] = 'error';
  }
}

/**
 * Form Submit handler
 * 
 * @see theme_editor_file_actions_upload_form()
 */
function theme_editor_file_actions_upload_form_submit($form, &$form_state) {
  $file = $form_state['file'];
  
  $form_state['redirect'] = _tep($file, 'file');  
  
}