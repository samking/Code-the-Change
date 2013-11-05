<?php
/**
 * @file Callback and form functions for hook_theme_editor_file_actions_info() actions in the file scope.
 */

/**
 * Callback funciton for the delete_file Action
 * 
 * @param $theme
 *   The Theme object
 * 
 * @param $file
 *   The file object
 * 
 * @return
 *   A populated Drupal Form.
 */
function theme_editor_file_actions_delete($theme, $file) {
  return drupal_get_form('theme_editor_file_actions_delete_form', $theme, $file);
}

/**
 * Form API function for delete-file action
 * 
 * @see theme_editor_file_actions_delete()
 */
function theme_editor_file_actions_delete_form($form, &$form_state, $theme, $file) {
  drupal_set_title('Delete ' . $file->name . ' from ' . $theme->info['name'] . ' theme.');
  $form['file'] = array(
    '#type' => 'value',
    '#value' => $file,
  );
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );
  $form['filename']['#markup'] = '<h4>' . $file->name . '</h4>';
  $options = array(
    'database' => t('Delete from database'),
  );
  if ($file->status) {
    $options['filesystem'] = t('Delete from filesystem');
  }
  $form['delete_options'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Delete options'),
    '#options' => $options,
    '#default_value' => array('fs', 'db'),
    '#description' => t('Please choose the deletion options you would like to use on this file.'),
  );
  if ($file->status) {
    $form['parent'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete parent directories'),
      '#states' => array(
        'visible' => array(
         ':input[name="delete_options[filesystem]"]' => array('checked' => TRUE)
        ),
        'checked' => array(
         ':input[name="delete_options[filesystem]"]' => array('checked' => TRUE)
        ),
      ),
      '#description' => t('Delete any empty parent directories in this files path. This can help ensure your thems filespace stays clean and uncluttered.')
    );
  }
   $form['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => 'Delete',
  );
  $form['buttons']['cancel'] = array(
    '#markup' => l('Cancel', _tep($file, 'file')),
  );
  return $form;
}

/**
 * Form API validate function for delete-file action
 * 
 * @see theme_editor_file_actions_delete_form()
 */
function theme_editor_file_actions_delete_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  $file = $values['file'];
  $error = NULL;
  if (!empty($values['delete_options']['filesystem'])) {
    theme_editor_file_delete($file, 'filesystem');
    if ($file->status == 1) {
      $error = t('The file could not be deleted from the filesystem. Please check that the file exists and is deletable.');
      form_set_error('delete_options', $error);
    }
    else {
      $dir = THEME_EDITOR_DIR . '/' . $file->theme . '/' . $file->path;
      theme_editor_rmdir($dir);
      drupal_set_message(t('!file has been removed from the file system.', array('!file' => $file->name)));
    }
  }
  if (!empty($values['delete_options']['database'])) {
    //needed to keep the file name for display after it has been removed
    $filename = $file->name;
    theme_editor_file_delete($file, 'database');
    if (empty($file->deleted)) {
      $error = t('The file could not be deleted from the database. Please try again.');
      form_set_error('delete_options', $error);
    }
    else {
      drupal_set_message(t('!file has been removed from the database.', array('!file' => $filename)));
    }
  }
}

/**
 * Form API submit function for delete-file action
 * 
 * @see theme_editor_file_actions_delete_form()
 */
function theme_editor_file_actions_delete_form_submit($form, &$form_state) {
  $form_state['redirect'] = _tep($form_state['values']['theme'], 'theme');
  if (empty($form_state['values']['delete_options']['database'])) {
    //the file was not deleted from the DB so we can redirect to it.
    $form_state['redirect'] = _tep($form_state['values']['file'], 'file');
  }
}

/**
 * Callback funciton for the revert_file Action
 * 
 * @param $theme
 *   The Theme object
 * 
 * @param $file
 *   The file object
 * 
 * @return
 *   A populated Drupal Form.
 */
function theme_editor_file_actions_revert($theme, $file) {
  $query = db_select('theme_editor_files_versions', 'v');
  $query->fields('v', array('vid', 'timestamp', 'uid', 'log'));
  $query->condition('fid', $file->fid);
  $query->orderBy('timestamp', 'DESC');
  $versions = $query->execute();
  return drupal_get_form('theme_editor_file_actions_revert_form', $theme, $file, $versions);
}

/**
 * FORM API for revert_file action
 * 
 * @todo
 *   build in a Diff system similair to diff module.
 */
function theme_editor_file_actions_revert_form($form, &$form_state, $theme, $file, $versions) {
 drupal_set_title('Revert ' . $file->name . ' to a previous version.');
  $form['file'] = array(
    '#type' => 'value',
    '#value' => $file,
  );
  $form['theme'] = array(
    '#type' => 'value',
    '#value' => $theme,
  );
  $form['filename']['#markup'] = '<h4>' . $file->name . '</h4>';
  $form['versions'] = array(
    '#type' => 'radios',
    '#title' => t('Select the version to revert to'),
  );
  $form['versions']['#theme'] = 'theme_editor_file_actions_revert_table';
  foreach ($versions as $version) {
    $form['versions']['#options'][$version->vid] = $version->vid;
    $form['versions']['#versions'][$version->vid] = $version;
  }
  $form['versions']['#default_value'] = $file->vid;
  $form['versions']['#headers'] = array(t('Revision'), t('Created on'), t('File Log'), t('Created by'));
  $form['buttons']['revert'] = array(
    '#type' => 'submit',
    '#value' => t('Revert File'),
  );
  $form['buttons']['cancel'] = array(
    '#markup' => l('Cancel', _tep($file, 'file')),
  );
  
  return $form;
}

/**
 * Form API submit function for revert-file action
 * 
 * @see theme_editor_file_actions_revert_form()
 */
function theme_editor_file_actions_revert_form_submit($form, &$form_state) {
  theme_editor_file_revert($form_state['values']['file'], $form_state['values']['versions']);
  drupal_set_message(t('File reverted to previous version'));
  $form_state['redirect'] = _tep($form_state['values']['file'], 'file');
}