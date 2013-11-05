<?php

/**
 * @file
 * Drush integration for the theme_editor_ide module.
 */

/**
 * Implements hook_drush_command().
 */
function theme_editor_ide_drush_command() {
  $items['theme-editor-ide-download'] = array(
    'description' => dt('Downloads the specified theme_editor_ide library.'),
    'aliases' => array('ide-dl'),
    'arguments' => array(
      'ide' => dt('The IDE library you wish to download or a comma seperated list of IDE\'s. For a list of available IDE\'s use use `drush theme-editor-ide-list`.'),  
    ),
  );
  $items['theme-editor-ide-list'] = array(
    'description' => dt('Lists the available theme_editor_ide libraries.'),
    'aliases' => array('ide-list'),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function theme_editor_ide_drush_help($section) {
  switch ($section) {
    case 'drush:theme-editor-ide-download':
      return dt('Downloads the specified theme_editor_ide library. Places it in the libraries directory. Skips download if library already present. This all happens automatically if you enable theme_editor_ide using drush.');
  }
}

/**
 * A command callback. Download the theme_editor_ide library using git.
 */
function drush_theme_editor_ide_download() {
  $path = drush_get_context('DRUSH_DRUPAL_ROOT');
  $args = func_get_args();
  
  foreach($args as $ide) {
    drush_log($ide . ' entered', 'ok');
  }
  return;
  $path .= '/'. libraries_get_path('theme_editor_ide');
  // If we have the libraries module go ahead and make the path.
  if (!is_dir($path)) {
    drush_mkdir($path);
  }

  $version = '7.x-1.x';
  $url = "https://github.com/arcaneadam/theme_editor_ide-Drupal/zipball/{$version}";
  $filepath = $path . '/theme_editor_ide.zip';

  if (file_exists($path . '/theme_editor_ide.swf')) {
    drush_log('theme_editor_ide already present. No download required.', 'ok');
    return;
  }
  //Download the file
  if (!drush_shell_exec('curl -L -o %s ' . $url, $filepath)) {
    return drush_set_error('theme_editor_ide_FETCH', dt('Drush was unable to download the theme_editor_ide library to @path.', array('@path' => $filepath)));
  }
  // Unzip it the file -- using the "update" option to avoid being prompted
  // about overwriting files.
  if (!drush_shell_exec("unzip -u {$filepath} -d {$path}")) {
    return drush_set_error('theme_editor_ide_UNTAR', dt('Drush was unable to untar the archive @path.', array('@path' => $filepath)));
  }
  if (!drush_shell_exec("mv {$path}/arcaneadam-theme_editor_ide-Drupal*/* {$path}")) {
    return drush_set_error('theme_editor_ide_MV', dt('Drush was unable to move the files to the appropriate directory'));
  }
  drush_shell_exec("rm -fR {$path}/arcaneadam-theme_editor_ide-Drupal*");

  drush_log(dt('theme_editor_ide library has been installed to @path.', array('@path' => $path)), 'success');
}

/**
 * Implements drush_MODULE_post_COMMAND().
 */
function drush_theme_editor_ide_post_pm_enable() {
  $modules = func_get_args();
  if (in_array('theme_editor_ide', $modules)) {
    drush_theme_editor_ide_download();
  }
}