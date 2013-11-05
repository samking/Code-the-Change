(function ($) {

Drupal.behaviors.themeEditorFieldsetSummaries = {
  attach: function (context) {
    $('fieldset.theme-editor-editor-versions', context).drupalSetSummary(function (context) {
      var revisionCheckbox = $('.form-item-revision input', context);

      // Return 'New revision' if the 'Create new revision' checkbox is checked,
      // or if the checkbox doesn't exist, but the revision log does. For users
      // without the "Administer content" permission the checkbox won't appear,
      // but the revision log will if the content type is set to auto-revision.
      if (revisionCheckbox.is(':checked') || (!revisionCheckbox.length && $('.form-item-log textarea', context).length)) {
        return Drupal.t('New Backup Revision');
      }

      return Drupal.t('No Backup');
    });

    $('fieldset.theme-editor-editor-options', context).drupalSetSummary(function (context) {
      var saveAs = $('select.theme-editor-editor-options-save-as option:selected', context).text();
        return Drupal.t(saveAs);
    });
    var $desc = $('fieldset.theme-editor-editor-actions', context).find('.fieldset-description').text();
    $('fieldset.theme-editor-editor-actions', context).drupalSetSummary(function (context) {
        return Drupal.t($desc);
    });
  }
};

})(jQuery);
