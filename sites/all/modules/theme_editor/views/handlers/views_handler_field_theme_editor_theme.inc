<?php
/**
 * @file
 * Contains the basic 'theme' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a theme.
 * Definition terms:
 * - link_to_theme default: Should this field have the checkbox "link to theme" enabled by default.
 */
class views_handler_field_theme_editor_theme extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_theme'])) {
      $this->additional_fields['theme'] = array('table' => 'theme_editor_files', 'field' => 'theme');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_theme'] = array('default' => isset($this->definition['link_to_theme default']) ? $this->definition['link_to_theme default'] : FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_theme'] = array(
      '#title' => t('Link this field to the Theme Editor'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_theme']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the file.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_theme'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $theme->name = $this->get_value($values, 'theme');
        $this->options['alter']['path'] = _tep($theme, 'theme');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    $value = theme_editor_load($value)->info['name'];
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
