<?php
/**
 * @file hooks and callbacks for file and theme access for theme editor
 * 
 * All file check hooks should cascade through to ALLOW.
 * They should primarily check for DENY flags and only return DENY on that instance.
 */

/**
 * Callback for theme_editor_theme_editor_access_records()
 * 
 * the functions called should return either THEME_EDITOR_FILE_ACCESS_DENY
 * or THEME_EDITOR_FILE_ACCESS_ALLOW after checking the file/theme
 */
function _theme_editor_access_records($file, $op, $account) {
  $function = '_theme_editor_access_' . $op;
  return call_user_func_array($function, array($file, $account));
}

/**
 * 
 */
function _theme_editor_access_create($file, $account) {
  //in create mode we don't get a fid so we need to determine if we are allowed to create files in this theme.
  return THEME_EDITOR_FILE_ACCESS_ALLOW;
}

/**
 * Determines weather or not an account has access to a certain file/theme
 * 
 * The file could be an actual file, or it could be just a theme ($file->theme)
 * We need to make this determination and then return the appropriate value
 */
function _theme_editor_access_edit($file, $account) {
  if (empty($file->fid) && !user_access('access_' . $file->theme . '_theme_editor', $account)) {
    //This is a theme so check the users access to this theme if not deny access
    return THEME_EDITOR_FILE_ACCESS_DENY;
  }
  
  return THEME_EDITOR_FILE_ACCESS_ALLOW;
}
/**
 * 
 */
function _theme_editor_access_delete($file, $account) {
  return THEME_EDITOR_FILE_ACCESS_ALLOW;
}

/**
 * Creates the default theme access form.
 */
function theme_editor_settings_access_form(&$form_state, $theme = '', $options = array()) {
  drupal_add_css(drupal_get_path('module', 'theme_editor') . '/css/access.css');
  drupal_add_js(drupal_get_path('module', 'theme_editor') . '/js/theme_editor_access.js');
  if (!empty($form_state['submitted']) && !empty($form_state['rebuild'])) {
    unset($form_state['values']['theme_editor']['access'], $form_state['input']['theme_editor']['access']);
  }
  $rights = _theme_editor_rebuild_perm_array($theme);
  if (!empty($theme) && user_access('configure_theme_editor')) {
    //this is a theme so we need to show options for allowing the global rights
    $global_rights = isset($form_state['#gloabl']) ? $form_state['#global'] : (isset($options['global_rights']) ? $options['global_rights'] : 1);
    $form['global_rights'] = array(
      '#type' => 'select',
      '#options' => array(
        1 => 'Inherit',
        0 => 'Ignore',
      ),
      '#title' => t('Global permissions handling'),
      '#default_value' => array($global_rights),
      '#description' => t('How the theme should handle the globally set rights') .
        '<p>' . t('If set to inherit, the theme will inherit the global access permissions. If set to ignore the theme will discard all global rights and only use it\'s own.') . '</p>',
    );
    if ($global_rights) {
      $rights = array_merge_recursive($rights, _theme_editor_rebuild_perm_array(NULL));
    }
  }
  $form['access'] = array(
    '#type' => 'fieldset',
    '#title' => t('Access settings'),
    '#collpsible' => TRUE,
    '#prefix' => '<div id="te-access-perms">',
    '#suffix' => '</div>',
  );
  $ops = array(
    'create' => t('Create'),
    'edit' => t('Edit'),
    'delete' => t('Delete'),
  );
  foreach (theme_editor_mime_types() as $type => $mime) {
    //For our mime_values we need to make then HTML safe
    //so that our draggable table can recognize them correctly.
    $mimes[htmlsafeclass($type)] = $mime;
  }
  asort($mimes);
  $roles = user_roles();
  $perms = module_invoke_all('permission');
  foreach ($perms as $key => $perm) {
    $perms[$key] = $perm['title'];
  }
  $form['access']['perms'] = array(
    '#type' => 'container',
    '#theme' => 'theme_editor_file_access_form',
    '#weight' => 5,
    '#access' => !empty($rights),
  );
  $form['access']['perms']['#mimes'] = $mimes;
  foreach ($rights as $right){
    //@TODO Somewhere in here before the next loop we need to sort by weight.
    //drupal_sort_weight()
    uasort($right, 'drupal_sort_weight');
    foreach ($right as $key => $perm) {
      $mime = htmlsafeclass($perm['mime']);
      $weight = $perm['weight'];
      $class = drupal_html_id('theme-editor-access-check-type-' . $mime . '-' . $weight);
      $form['access']['perms'][$key]['rid'] = array(
        '#type' => 'value',
        '#value' => $perm['rid'],
      );
      $form['access']['perms'][$key]['mime'] = array(
        '#type' => 'select',
        '#options' => $mimes,
        '#title' => t('Mime Type'),
        '#default_value' => array($mime),
      );
      $form['access']['perms'][$key]['weight'] = array(
        '#type' => 'select',
        '#options' => range(0,10),
        '#title' => t('Weight'),
        '#default_value' => array($weight),
      );
      $form['access']['perms'][$key]['ops'] = array(
        '#type' => 'checkboxes',
        '#options' => $ops,
        '#title' => t('Operations'),
        '#default_value' => array_keys($perm['ops'], 1),
      );
      $form['access']['perms'][$key]['check_type'] = array(
        '#type' => 'select',
        '#options' => array(
          'role' => t('By Role'),
          'perm' => t('By User Permission'),
          'user' => t('By User'),
          'php' => t('PHP Code'),
        ),
        '#title' => t('Access Check Type'),
        '#attributes' => array(
          'class' => array($class),
        ),
        '#default_value' => array($perm['check_type']),
      );
      $form['access']['perms'][$key]['check_types']['role'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#size' => 5,
        '#options' => $roles,
        '#title' => t('Roles'),
        '#states' => array(
          'visible' => array(
            '.' . $class => array('value' => 'role'),
          ),
        ),
        '#default_value' => !empty($perm['check_types']['role']) ? array_values($perm['check_types']['role']) : array(),
      );
      $form['access']['perms'][$key]['check_types']['perm'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#size' => 5,
        '#options' => $perms,
        '#title' => t('Permissions'),
        '#states' => array(
          'visible' => array(
            '.' . $class => array('value' => 'perm'),
          ),
        ),
        '#default_value' => !empty($perm['check_types']['perm']) ? array_values($perm['check_types']['perm']) : array(),
      );
      $form['access']['perms'][$key]['check_types']['user'] = array(
        '#type' => 'textfield',
        '#title' => t('Users'),
        '#description' => t('Enter Usernames seperated by comma'),
        '#autocomplete_path' => 'user/autocomplete_multiple',
        '#size' => 32,
        '#states' => array(
          'visible' => array(
            '.' . $class => array('value' => 'user'),
          ),
        ),
        '#default_value' => !empty($perm['check_types']['user']) ? $perm['check_types']['user'] : '',
      );
      $form['access']['perms'][$key]['check_types']['php'] = array(
        '#type' => 'textarea',
        '#cols' => 10,
        '#rows' => 10,
        '#title' => t('PHP Code'),
        '#description' => t('Do not include the <?php ?> tags. The code should return TRUE to grant access to a the file, or FALSE to deny access. !br
                             Available variables to check against are $file (object containing fid, vid, theme, mime, name, path, created, changed, log, uid, data, etc.) and $account (a user object).',
                             array('!br' => '<br>')
                           ),
        '#states' => array(
          'visible' => array(
            '.' . $class => array('value' => 'php'),
          ),
        ),
        '#default_value' => !empty($perm['check_types']['php']) ? $perm['check_types']['php'] : '',
      );
      $form['access']['perms'][$key]['action'] = array(
        '#type' => 'submit',
        '#name' => $key,
        '#value' => t('Delete'),
        '#submit' => array('theme_editor_settings_access_form_delete_submit'),
        '#ajax' => array(
          'callback' => 'theme_editor_access_ajax',
          'wrapper' => 'te-access-perms',
          'progress' => array(
            'type' => 'throbber',
            'message' => t('Deleting Permission'),  
          ),
          'method' => 'replace',
          'effect' => 'fade',
        ),
      );
      if (empty($perm['theme']) && !empty($theme)) {
        //this is a global option mark it as such so the user knows they will be deleting it from the global options as well
        $form['access']['perms'][$key]['#global'] = TRUE;
      }
    }
  }
  
  //Create the Add Access Form
  $form['access']['_add_'] = array(
    '#type' => 'container',
    '#title' => 'Add File Access Permission',
    '#theme' => 'theme_editor_file_access_table',
    '#tree' => TRUE,
    '#weight' => 0,
  );
  $form['access']['_add_'][0]['mime'] = array(
    '#type' => 'select',
    '#options' => $mimes,
    '#title' => t('Mime Type'),
  );
  $form['access']['_add_'][0]['ops'] = array(
    '#type' => 'checkboxes',
    '#options' => $ops,
    '#title' => t('Operations'),
  );
  $form['access']['_add_'][0]['check_type'] = array(
    '#type' => 'select',
    '#options' => array(
      'role' => t('By Role'),
      'perm' => t('By User Permission'),
      'user' => t('By User'),
      'php' => t('PHP Code'),
    ),
    '#title' => t('Access Check Type'),
    '#attributes' => array(
      'class' => array('theme-editor-access-check-type'),
    ),
  );
  $form['access']['_add_'][0]['check_types']['role'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => 5,
    '#options' => $roles,
    '#title' => t('Roles'),
    '#states' => array(
      'visible' => array(
        '.theme-editor-access-check-type' => array('value' => 'role'),
      ),
    ),
  );
  $form['access']['_add_'][0]['check_types']['perm'] = array(
    '#type' => 'select',
    '#multiple' => TRUE,
    '#size' => 5,
    '#options' => $perms,
    '#title' => t('Permissions'),
    '#states' => array(
      'visible' => array(
        '.theme-editor-access-check-type' => array('value' => 'perm'),
      ),
    ),
  );
  $form['access']['_add_'][0]['check_types']['user'] = array(
    '#type' => 'textfield',
    '#title' => t('Users'),
    '#description' => t('Enter Usernames seperated by comma'),
    '#autocomplete_path' => 'user/autocomplete_multiple',
    '#size' => 32,
    '#states' => array(
      'visible' => array(
        '.theme-editor-access-check-type' => array('value' => 'user'),
      ),
    ),
  );
  $form['access']['_add_'][0]['check_types']['php'] = array(
    '#type' => 'textarea',
    '#cols' => 10,
    '#rows' => 10,
    '#title' => t('PHP Code'),
    '#description' => t('Do not include the <?php ?> tags. The code should return TRUE to grant access to a the file, or FALSE to deny access. !br
                         Available variables to check against are $file (object containing fid, vid, theme, mime, name, path, created, changed, log, uid, data, etc.) and $account (a user object).',
                         array('!br' => '<br>')
                        ),
    '#states' => array(
      'visible' => array(
        '.theme-editor-access-check-type' => array('value' => 'php'),
      ),
    ),
  );
  $form['access']['_add_'][0]['action'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#submit' => array('theme_editor_settings_access_form_add_submit'),
    '#ajax' => array(
      'callback' => 'theme_editor_access_ajax',
      'wrapper' => 'te-access-perms',
      'progress' => array(
        'type' => 'throbber',
        'message' => t('Adding Permission'),  
      ),
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  return $form;
}

/**
 * Add file callback for the black add/edit form
 */
function _theme_editor_settings_access_form_add_submit($form, &$form_state) {
  $access = $form_state['values']['theme_editor']['access']['_add_'][0];
  $mime = $access['mime'];
  $weight = isset($form_state['te_access'][$mime]) ? count($form_state['te_access'][$mime]) : 0;
  $key = $mime . '--' . $weight;
  $access['weight'] = $weight;
  $access['theme'] = isset($form_state['values']['theme']) ? $form_state['values']['theme']: NULL;
  _theme_editor_access_right_save($access);
  $form_state['#gloabl'] = $form_state['values']['theme_editor']['global_rights'];
  $form_state['rebuild'] = TRUE;
  drupal_set_message('Permission Added');
}

/**
 * Delete permissions for file_access
 */
function _theme_editor_settings_access_form_delete_submit($form, &$form_state) {
  $parents = $form_state['clicked_button']['#parents'];
  array_pop($parents);
  $key = array_pop($parents);
  $rid = $form_state['values']['theme_editor']['access']['perms'][$key]['rid'];
  db_delete('theme_editor_file_access_rights')
    ->condition('rid', $rid)
    ->execute();
  $form_state['#gloabl'] = $form_state['values']['theme_editor']['global_rights'];
  $form_state['rebuild'] = TRUE;
  drupal_set_message('Permission removed');
}

/**
 * Form submit handler for file_access
 */
function _theme_editor_settings_access_form_submit($form, &$form_state){
  $perms = $form_state['values']['theme_editor']['access']['perms'];
  foreach ($perms as $perm) {
    _theme_editor_access_right_save($perm);
  }
  unset($form_state['values']['theme_editor']['access']);
}

/**
 * Reorder the te_access array from an array of form values
 */
function _theme_editor_rebuild_perm_array($theme = NULL) {
  $perms = array();
  $rights = _theme_editor_get_file_rights($theme);
  foreach ($rights as $right) {
    $perm = array();
    $perm['ops'] = array(
      'create' => $right->create_op,
      'edit' => $right->edit_op,
      'delete' => $right->delete_op,
    );
    $arg = $right->check_type;
    if (in_array($arg, array('role', 'perm'))) {
     $perm['check_types'][$arg] = unserialize($right->arg);
    }
    else {
      $perm['check_types'][$arg] = $right->arg;
    }
    $mime = $right->mime;
    $rid = $right->rid;
    //Unset some extra items before we add the right
    unset($right->arg, $right->create_op, $right->edit->op, $right->delete->op);
    $perm += (array)$right;
    $perms[$mime][$rid] = $perm;
  }
  return $perms;
}

/**
 * Loads all file rights for a given theme, blank theme defaults to global
 */
function _theme_editor_get_file_rights($theme = NULL) {
  $query = db_select('theme_editor_file_access_rights', 'tef');
  if ($theme) {
    $query->condition('theme', $theme, '=');
  }
  else {
    //get the global settings
    $query->isNull('theme');
  }
  $query->fields('tef')
        ->orderBy('mime', 'ASC')
        ->orderBy('weight', 'ASC');
  return $query->execute();
}

/**
 * Saves a file permission setting to the file rights table
 */
function _theme_editor_access_right_save(&$right) {
  if (is_array($right)) {
    //cast as an object for saving purposes
    //we'll return it to an array if we need to afterwards
    $array = TRUE;
    $right = (object)$right;
  }
  $up = array('rid');
  if (isset($right->is_new) || empty($right->rid)) {
    //if this is a new rd then we simply empty the $up varaible that is passed to drupal_write_record
    $up = array();
  }
  if(is_array($right->ops)) {
    //we have an array of ops convert them to the appropriate keys
    foreach ($right->ops as $key => $value) {
      $key_op = $key . '_op'; //See comment in .install about why this is needed
      $right->$key_op = (int)(boolean)$value;
    }
  }
  $check_type = $right->check_type;
  if (in_array($check_type, array('role', 'perm'))) {
    $arg = serialize($right->check_types[$check_type]);
  }
  else {
    $arg = $right->check_types[$check_type];
  }
  $right->arg = $arg;
  //next we are going to do some checking on the mime type
  //to make sure it's safe and in the correct format for saving
  $mimes = array_keys(theme_editor_mime_types());
  if (!in_array($right->mime, $mimes)) {
    foreach ($mimes as $mime) {
      //This should ensure that we get a correct mime saved to the DB
      //There is always the chance that this could get munged up though.
      if (htmlsafeclass($mime) == $right->mime) {
        $right->mime = $mime;
      }
    }
  }
  //At this point we should be ready to save the right
  drupal_write_record('theme_editor_file_access_rights', $right, $up);
  
  if (isset($array)) {
    //If the right was an array when I got passed in return it to that state
    $right = (array)$right;
  }
}

/**
 * Menu callback; Retrieve a JSON object containing autocomplete suggestions for existing users.
 */
function user_autocomplete_multiple($string = '') {
  $matches = array();
  if ($string) {
    $strings = explode(',', $string);
    $string = array_pop($strings);
    $query = db_select('users')
               ->fields('users', array('name'))
               ->condition('name', db_like(trim($string)) . '%', 'LIKE')
               ->range(0, 10);
    if (count($strings) > 0) {
      $query->condition('name', $strings, 'NOT IN');
    }
    $result = $query->execute();
    foreach ($result as $account) {
      $key = $account->name;
      if (count($strings) > 0) {
        $key = implode(',', $strings) . ', ' . $account->name;
      }
      $matches[$key] = check_plain($account->name);
    }
  }

  drupal_json_output($matches);
}

/**
 * Theme function for the file access table display
 */
function theme_theme_editor_file_access_table($vars) {
  $form = $vars['form'];
  $tvars['caption'] = $form['#title'];
  $tvars['header'] = array(
    t('Mime Type'),
    t('Operations'),
    t('Access Check Type'),
    t('Access Arguments'),
    '',
  );
  foreach (element_children($form) as $delta) {
    foreach (array('mime','ops','check_type') as $key) {
      unset($form[$delta][$key]['#title']);
    }
    $action = drupal_render($form[$delta]['action']);
    $check_types = drupal_render_children($form[$delta]['check_types']);
    $tvars['rows'][] = array(
      drupal_render($form[$delta]['mime']),
      drupal_render($form[$delta]['ops']),
      drupal_render($form[$delta]['check_type']),
      $check_types,
      $action,
     );
  }
  return theme('table', $tvars) .  drupal_render_children($form);
}

/**
 * Processes variables for theme-editor-file-access-form.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $form
 *
 * @see theme-editor-file-access-form.tpl.php
 */
function template_preprocess_theme_editor_file_access_form(&$variables) {
  $variables['mimes'] = $variables['form']['#mimes'];
  
  $variables['description'] = t('');
  foreach ($variables['mimes'] as $key => $value) {
    // Initialize an empty array for the mime.
    $variables['perms_listing'][$key] = array();
  }
  
  // Add each perm in the form to the appropriate place in the block listing.
  foreach (element_children($variables['form']) as $i) {
    $perm = &$variables['form'][$i];
    // Fetch the region for the current block.
    $mime = $perm['mime']['#default_value'][0];
    foreach (array('mime','ops','check_type', 'weight') as $key) {
      unset($perm[$key]['#title']);
    }
    
    // Set special classes needed for table drag and drop.
    $perm['mime']['#attributes']['class'] = array('perm-mime-select', 'perm-mime-' . $mime);
    $perm['weight']['#attributes']['class'] = array('perm-weight', 'perm-weight-' . $mime);
    
    $variables['perms_listing'][$mime][$i] = new stdClass();
    $variables['perms_listing'][$mime][$i]->mime = drupal_render($perm['mime']);
    $variables['perms_listing'][$mime][$i]->weight = drupal_render($perm['weight']);
    $variables['perms_listing'][$mime][$i]->ops = drupal_render($perm['ops']);
    $variables['perms_listing'][$mime][$i]->check = drupal_render($perm['check_type']);
    $variables['perms_listing'][$mime][$i]->args = drupal_render_children($perm['check_types']);
    $variables['perms_listing'][$mime][$i]->action = drupal_render($perm['action']);
    if (isset($perm['#global'])) {
      //this is a global so lets make sure we properly mark it as sucj for the end user.
      $variables['perms_listing'][$mime][$i]->global = t('Gloabl permission.');
    }
  }
  $variables['form_extra'] = drupal_render_children($variables['form']);
}

/**
 * Builds a html safe string for use in classes
 */
function htmlsafeclass($id) {
  $id = strtr(drupal_strtolower($id), array(' ' => '-', '_' => '-', '[' => '-', ']' => ''));

  // As defined in http://www.w3.org/TR/html4/types.html#type-name, HTML IDs can
  // only contain letters, digits ([0-9]), hyphens ("-"), underscores ("_"),
  // colons (":"), and periods ("."). We strip out any character not in that
  // list. Note that the CSS spec doesn't allow colons or periods in identifiers
  // (http://www.w3.org/TR/CSS21/syndata.html#characters), so we strip those two
  // characters as well.
  $id = preg_replace('/[^A-Za-z0-9\-_]/', '', $id);

  // Removing multiple consecutive hyphens.
  $id = preg_replace('/\-+/', '-', $id);
  
  return $id;
}
