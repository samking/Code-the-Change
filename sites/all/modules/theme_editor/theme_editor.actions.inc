<?php
/**
 * @file Various file actions for a theme. Such as add, upload, and delete
 */

/**
 * Callback for the various file_actions
 * 
 * @param $theme
 *   The Theme object from theme_editor_load
 * 
 * @param $op
 *   The operation to act upon.
 */
function theme_editor_file_actions($theme, $op) {
  
  $actions = theme_editor_get_file_actions();
  if (empty($actions[$op])) {
    drupal_not_found();
  }
  $action = $actions[$op];
  if (!empty($action['file'])) {
    $path = explode('/', $action['file']);
    $file = array_pop($path);
    $path = implode('/', $path);
    $file = explode('.', $file);
    $extension = array_pop($file);
    $file = implode('.', $file);
    $file = !$path ? $file : $path . '/' . $file;
    module_load_include($extension, $action['module'], $file);
  }
  if (function_exists($action['callback'])) {
    $args['theme'] = $theme;
    if($action['scope'] == 'file') {
      $fid = arg(7);
      $args['file'] = $file = theme_editor_file_load($fid);
    }
    drupal_alter('theme_editor_file_action', $action, $args);
    drupal_set_title($action['title']);
    return call_user_func_array($action['callback'], $args);
  }
}
