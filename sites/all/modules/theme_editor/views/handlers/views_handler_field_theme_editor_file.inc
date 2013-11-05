<?php
/**
 * @file
 * Contains the basic 'file' field handler.
 */

/**
 * Field handler to provide simple renderer that allows linking to a node.
 * Definition terms:
 * - link_to_file default: Should this field have the checkbox "link to file" enabled by default.
 */
class views_handler_field_theme_editor_file extends views_handler_field {

  function init(&$view, &$options) {
    parent::init($view, $options);
    if (!empty($this->options['link_to_file'])) {
      $this->additional_fields['fid'] = array('table' => 'theme_editor_files', 'field' => 'fid');
      $this->additional_fields['theme'] = array('table' => 'theme_editor_files', 'field' => 'theme');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_file'] = array('default' => isset($this->definition['link_to_file default']) ? $this->definition['link_to_file default'] : FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  function options_form(&$form, &$form_state) {
    $form['link_to_file'] = array(
      '#title' => t('Link this field to the file editor'),
      '#description' => t("Enable to override this field's links."),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_file']),
    );

    parent::options_form($form, $form_state);
  }

  /**
   * Render whatever the data is as a link to the file.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_file'])) {
      if ($data !== NULL && $data !== '') {
        $this->options['alter']['make_link'] = TRUE;
        $this->options['alter']['path'] = 'admin/appearance/editor/theme/' . $this->get_value($values, 'theme') . '/' . $this->get_value($values, 'fid');
      }
      else {
        $this->options['alter']['make_link'] = FALSE;
      }
    }
    return $data;
  }

  function render($values) {
    $value = $this->get_value($values);
    return $this->render_link($this->sanitize_value($value), $values);
  }
}
