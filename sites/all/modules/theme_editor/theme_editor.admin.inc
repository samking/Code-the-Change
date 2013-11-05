<?php
/**
 * @file Various functions and callbacks related to Theme Editor administration functionality
 */

/**
 * Creates the Theme Editor toggle confirmation form
 */
function theme_editor_toggle($form, $form_state, $theme, $switch = 'disable', $path = 'admin/appearance') {
  $form['#theme_editor'] = $theme;
  $form['theme_name'] = array('#type' => 'value', '#value' => $theme->name);
  $form['switch'] = array('#type' => 'value', '#value' => $switch);
  return confirm_form($form,
    t('Are you sure you want to @status the Theme Editor Changes for !theme', array('@status' => ucfirst($switch), '!theme' => $theme->info['name'])),
    $path,
    t('You can undo/redo this at any time from the Apearance page.'),
    t('@status', array('@status' => ucfirst($switch))),
    t('Cancel')
  );
}

/**
 * Submit Callback for toggle confirmation form.
 */
function theme_editor_toggle_submit(&$form, &$form_state) {
  if ($form_state['values']['confirm']) {
    _theme_editor_toggle($form['#theme_editor'], $form_state['values']['switch']);
    //Make sure the theme data gets rebuilt and the theme cache gets cleared.
    _theme_editor_rebuild_theme_data();
  }
  $form_state['redirect'] = 'admin/appearance';
}

/**
 * Form Callback for Settings Page
 */
function theme_editor_settings($form, $form_state) {
  $global = _te_vars('theme_editor_global');
  $form['theme_editor_directory'] = array(
    '#type' => 'textfield',
    '#title' => t('Directory for edited themes'),
    '#default_value' => _te_vars('theme_editor_directory'),
    '#description' => t('The directory that overwritten themes will be stored in. By default this set up in the public files directory, but should be changed to a directory that is inside sites/* and is writeable by the server'),
  );
  $form['theme_editor_global'] = array(
    '#type' => 'fieldset',
    '#title' => t('Global Settings'),
    '#collapsible' => TRUE,
    '#tree' => TRUE,
  );
  $form['theme_editor_global'] += theme_editor_settings_subform($form_state, $global);
  foreach (module_implements('theme_editor_settings') as $module) {
    $settings = array();
    $function = $module . '_theme_editor_settings';
    $settings = $function(NULL, TRUE);
    if (is_array($settings)) {
      $form['theme_editor_additional'][$module] = array(
        '#tree' => TRUE
      );
      $form['theme_editor_additional'][$module] += $settings;
    }
  }
  $form['theme_editor'] = array('#tree' => TRUE);
  module_load_include('inc', 'theme_editor', 'theme_editor.access');
  $form['theme_editor'] += theme_editor_settings_access_form($form_state);
  $form['#submit'][] = 'theme_editor_theme_settings_submit';
  
  return system_settings_form($form);
}

/**
 * Form for global and theme based settings
 */
function theme_editor_settings_subform(&$form_state, $global) {
  $form['revision'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create new Revision'),
    '#default_value' => $global['revision'],
  );
  
  $form['save_as'] = array(
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
  
  return $form;
}