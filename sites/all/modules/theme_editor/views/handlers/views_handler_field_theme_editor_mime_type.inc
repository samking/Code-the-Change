<?php
/**
 * A handler to provide proper displays for dates.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_theme_editor_mime_type extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();

    $options['mime_format'] = array('default' => 'human');

    return $options;
  }

  function options_form(&$form, &$form_state) {

    $date_formats = array();
    $date_types = system_get_date_types();
    foreach ($date_types as $key => $value) {
      $date_formats[$value['type']] = check_plain(t($value['title'] . ' format')) . ': ' . format_date(REQUEST_TIME, $value['type']);
    }

    $form['mime_format'] = array(
      '#type' => 'select',
      '#title' => t('Mime Type format'),
      '#options' => array(
        'human' => 'Human Readable (CSS, PNG, JavaScript)',
        'machine' => 'Machine (text/css, image/png, application/javascript)',
      ),
      '#default_value' => isset($this->options['mime_format']) ? $this->options['mime_format'] : 'human',
    );

    parent::options_form($form, $form_state);
  }

  function render($values) {
    $value = $this->get_value($values);
    $format = $this->options['mime_format'];
    if ($format == 'human') {
      return theme_editor_mime_type_render($value);
    }
    return $value;
  }
}