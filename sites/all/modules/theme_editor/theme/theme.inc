<?php
/**
 * @file - Contains all the hook_theme functions needed.
 */

/**
 * Returns HTML for the revert_file form.
 *
 * @param $variables
 *   An associative array containing:
 *   - form: A render element representing the form.
 *
 * @ingroup themeable
 */
function theme_theme_editor_file_actions_revert_table($variables) {
  $form = $variables['form'];
  
  // Individual table headers.
  $rows = array();
  //we are going to store the accounts so we don't unnessicarily call user_load
  $accounts = array();
  $current = $form['#default_value'];
  // Iterate through all the versions, which are
  // children of this radioset.
  foreach (element_children($form) as $vid) {
    // Stick it into $version_radio and $version for easier accessing.    
    $version_radio = $form[$vid];
    $version = $form['#versions'][$vid];
    $row = array();
    $astrik = '';
    if ($current == $vid) {
      $astrik = '<span class="form-required" title="' . t('This is the current revision') . '">*</span>';
      $version_radio['#title'] .= $astrik;
      $version_radio['#attributes'] = array('disabled' => TRUE);
      $version_radio['#value'] = FALSE;
    }
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($version_radio));
    $label = '<label';
    if (isset($version_radio['#id'])) {
      $label .= ' for="' . $version_radio['#id'] . '"';
    }
    $row[] = $label . '><strong>' . format_date($version->timestamp) . '</strong></label>';
    $log = array('#markup' => '<div>' . $version->log . '</div>');
    $row[] = drupal_render($log);
    if (!isset($accounts[$version->uid])) {
      $account = user_load($version->uid);
      $accounts[$version->uid]  = $account;
    }
    else {
      $account = $accounts[$version->uid];
    }
    $row[] = theme('username', array('account' => $account));
    $rows[] = $row;
  }
  $astrik_description = '<div class="description"><span class="form-required" title="This is the current revision.">*</span> - ' . t('The active revision') . '</div>';
  return theme('table', array('header' => $form['#headers'], 'rows' => $rows)) . $astrik_description;
}

/**
 * Returns HTML for the revert_file form.
 *
 * @param $variables
 *   An associative array containing:
 *   - file: A file object.
 *   - options: an options array that contains
 *     - wrapper: A wrapper HTML tag for the output. i.e. div, span, h3. Defaults to div.
 *     - info wrapper: A wrapper HTML tag for each info element. Defaults to span. See previous.
 *     - attributes: Drupal attributes for the main wrapper
 *     - info attributes: An array keyed on the object key of the $file for which these attributes are supposed to apply Example:
 *       - $options['info_attributes']['name'] = array('class' => array('filename'));
 *     - hide: An of the object key of the $file flagging fields to be hidden from display.
 *     - format: An array keyed on the object key of the $file containing an array of a fomating function for the field and arguments to pass along with the value.
 *       If none is defined then the field will simply be run through check_plain.
 *     - label: An array keyed on the object key of the $file containing alabel to place to the left of the value.
 *
 * @ingroup themeable
 */
function theme_theme_editor_file_info($variables) {
  $file = $variables['file'];
  $options = array(
    'wrapper' => 'div',
    'info wrapper' => 'span',
    'attributes' => array(
      'class' => array('theme-editor-file-info')
    ),
    'info attributes' => array(),
    'hide' => array('fid', 'vid', 'log', 'data', 'base64', 'timestamp'),
    'format' => array(
      'created' => array('function' => 'format_date'),
      'changed' => array('function' => 'format_date'),
      'uid' => array('function' => 'theme_editor_format_username'),
      'status' => array('function' => 'theme_editor_format_file_status'),
      'mime' => array('function' => 'theme_editor_mime_type_render'),
      'theme' => array('function' => 'theme_editor_load'),
    ),
    'label' => array(
      'created' => t('Created on'),
      'changed' => t('Last changed'),
      'uid' => t('Author'),
      'mime' => t('File Type'),
      'theme' => t('Theme'),
      'path' => t('Path'),
    )
  );
  $options = array_merge($variables['options'], $options);
  
  $render = array();
  $info = '';
  foreach ($file as $key => $value) {
    $out = '';
    if (!in_array($key, $options['hide'])) {
      if (isset($options['format'][$key])) {
        //if there is a formating function we should use here then do it.
        $format = $options['format'][$key];
        //First we push our $value to the front of the array for the call_user_func_array
        array_unshift($format, $value);
        //next we pull out the function and unset that variable
        $function = $format['function'];
        unset($format['function']);
        //We should now be left with only the args to pass to the function in $format
        $out = call_user_func_array($function, $format);
        if ($key == 'theme') {
          //make a special exception for the theme so we don't have to write a whole new function for it.
          $out = $out->info['name'];
        }
      }
      else {
        $out = check_plain($value);
      }
      if (!empty($out)) {
        $options['info attributes'][$key]['class'][] = 'theme-editor-file-info-element-' . $key;
        $render[$key] = theme('theme_editor_file_info_element',array(
          'out' => $out,
          'wrapper' => $options['info wrapper'],
          'attributes' => $options['info attributes'][$key],
          'label' => isset($options['label'][$key]) ? $options['label'][$key] : NULL,
        ));
      }
    }
  }
  return '<' . $options['wrapper'] . drupal_attributes($options['attributes']) . '>' . implode("\n | \n", $render) . '</' . $options['wrapper'] . '>';
}

/**
 * Themes a theme_editor_file_info_element
 * 
 * @params $variables
 *   an associative array containing
 *     - out: The value to render out.
 *     - wrapper: The HTML Element to wrap the out variable in.
 *     - attributes: A drupal attributes array
 *     - label: a label to place next to the element.
 * 
 * @return
 *   The themed output of the element;
 */
function theme_theme_editor_file_info_element($variables) {
  $label = '';
  if(!empty($variables['label'])) {
    $label = '<strong>' . $variables['label'] . ':</strong> ';
  }
  return '<' . $variables['wrapper'] . drupal_attributes($variables['attributes']) . '>' . $label . $variables['out'] . '</' . $variables['wrapper'] . '>';
}

/**
 * Formats a passed uid into a username
 * 
 * @param $uid
 *   The uid to load a user object from
 * 
 * @return
 *   A themed username
 */
function theme_editor_format_username($uid) {
  return theme('username', array('account' => user_load($uid)));
}

/**
 * Formats a passed status into a string
 * 
 * @param $status
 *   The boolean status of the file.
 * 
 * @return
 *   A themed status
 */
function theme_editor_format_file_status($status) {
  $file_status = array(
    0 => t('Database only'),
    1 => t('File exists in filesystem'),
  );
  return $file_status[$status];
}