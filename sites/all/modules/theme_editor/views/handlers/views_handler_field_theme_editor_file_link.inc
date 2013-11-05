<?php
/**
 * Field handler to present a link to the node.
 */
class views_handler_field_theme_editor_file_link extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['fid'] = 'fid';
    $this->additional_fields['theme'] = 'theme';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $data['fid'] = $this->get_value($values, 'fid');
    $data['theme'] = $this->get_value($values, 'theme');
    return $this->render_link($data, $values);
  }

  function render_link($data, $values) {
    $this->options['alter']['make_link'] = TRUE;
    $file->fid = $this->sanitize_value($data['fid']);
    $file->theme = $this->sanitize_value($data['theme']);
    $this->options['alter']['path'] = _tep($file, 'file');
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    return $text;
  }
}
