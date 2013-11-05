<?php
/**
 * Field handler to present a link node edit.
 */
class views_handler_field_theme_editor_file_revert_link extends views_handler_field_theme_editor_file_link {
  function construct() {
    parent::construct();
  }

  function render_link($data, $values) {
    //@todo ensure user has access to revert this file
    $theme = $this->sanitize_value($data['theme']);
    $fid = $this->sanitize_value($data['fid']);
    $this->options['alter']['make_link'] = TRUE;
    $this->options['alter']['path'] = "admin/appearance/editor/theme/$theme/file_actions/revert_file/$fid";
    $this->options['alter']['query'] = drupal_get_destination();

    $text = !empty($this->options['text']) ? $this->options['text'] : t('revert');
    return $text;
  }
}
