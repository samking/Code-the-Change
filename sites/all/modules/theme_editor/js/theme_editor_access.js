(function($) {

  /**
   * Move a permission in the permissionss table from one mime type to another
   * via select list.
   *
   * This behavior is dependent on the tableDrag behavior, since it uses the
   * objects initialized in that behavior to update the row.
   */
  Drupal.behaviors.themeEditorAccessDrag = {
    attach : function(context, settings) {
      // tableDrag is required and we should be on the blocks admin page.
      if( typeof Drupal.tableDrag == 'undefined' || typeof Drupal.tableDrag.te_perms == 'undefined') {
        return;
      }

      var table = $('table#te_perms');
      var tableDrag = Drupal.tableDrag.te_perms;
      // Get the blocks tableDrag object.

      // Add a handler for when a row is swapped, update empty regions.
      tableDrag.row.prototype.onSwap = function(swappedRow) {
        checkEmptyRegions(table, this);
      };
      // A custom message for the blocks page specifically.
      Drupal.theme.tableDragChangedWarning = function() {
        return '<div class="messages warning">' + Drupal.theme('tableDragChangedMarker') + ' ' + Drupal.t('The changes to these permissions will not be saved until the <em>Save Configuration</em> button is clicked.') + '</div>';
      };
      // Add a handler so when a row is dropped, update fields dropped into new
      // regions.
      tableDrag.onDrop = function() {
        dragObject = this;
        // Use "region-message" row instead of "region" row because
        // "region-{region_name}-message" is less prone to regexp match errors.
        var regionRow = $(dragObject.rowObject.element).prevAll('tr.region-message').get(0);
        var regionName = regionRow.className.replace(/([^ ]+[ ]+)*region-([^ ]+)-message([ ]+[^ ]+)*/, '$2');
        var regionField = $('select.perm-mime-select', dragObject.rowObject.element);
        // Check whether the newly picked region is available for this block.
        if($('option[value=' + regionName + ']', regionField).length == 0) {
          // If not, alert the user and keep the block in its old region setting.
          alert(Drupal.t('The permission cannot be placed in this region.'));
          // Simulate that there was a selected element change, so the row is put
          // back to from where the user tried to drag it.
          regionField.change();
        }
        else if($(dragObject.rowObject.element).prev('tr').is('.region-message')) {
          var weightField = $('select.perm-weight', dragObject.rowObject.element);
          var oldRegionName = weightField[0].className.replace(/([^ ]+[ ]+)*perm-weight-([^ ]+)([ ]+[^ ]+)*/, '$2');

          if(!regionField.is('.perm-mime-' + regionName)) {
            regionField.removeClass('perm-mime-' + oldRegionName).addClass('perm-mime-' + regionName);
            weightField.removeClass('perm-weight-' + oldRegionName).addClass('perm-weight-' + regionName);
            regionField.val(regionName);
          }
        }
      };
      // Add the behavior to each region select list.
      $('select.perm-mime-select', context).once('perm-mime-select', function() {
        $(this).change(function(event) {
          // Make our new row and select field.
          var row = $(this).parents('tr:first');
          var select = $(this);
          tableDrag.rowObject = new tableDrag.row(row);
          // Find the correct region and insert the row as the first in the
          // region.
          $('tr.region-message', table).each(function() {
            if($(this).is('.region-' + select[0].value + '-message')) {
              // Add the new row and remove the old one.
              $(this).after(row);
              // Manually update weights and restripe.
              tableDrag.updateFields(row.get(0));
              tableDrag.rowObject.changed = true;
              if(tableDrag.oldRowElement) {
                $(tableDrag.oldRowElement).removeClass('drag-previous');
              }
              tableDrag.oldRowElement = row.get(0);
              tableDrag.restripeTable();
              tableDrag.rowObject.markChanged();
              tableDrag.oldRowElement = row;
              $(row).addClass('drag-previous');
            }
          });
          // Modify empty regions with added or removed fields.
          checkEmptyRegions(table, row);
          // Remove focus from selectbox.
          select.get(0).blur();
        });
      });
      var checkEmptyRegions = function(table, rowObject) {
        $('tr.region-message', table).each(function() {
          // If the dragged row is in this region, but above the message row,
          // swap it down one space.
          if($(this).prev('tr').get(0) == rowObject.element) {
            // Prevent a recursion problem when using the keyboard to move rows
            // up.
            if((rowObject.method != 'keyboard' || rowObject.direction == 'down')) {
              rowObject.swap('after', this);
            }
          }
          // This region has become empty.
          if($(this).next('tr').is(':not(.draggable)') || $(this).next('tr').size() == 0) {
            $(this).removeClass('region-populated').addClass('region-empty');
          }
          // This region has become populated.
          else if($(this).is('.region-empty')) {
            $(this).removeClass('region-empty').addClass('region-populated');
          }
        });
      };
    }
  };

})(jQuery);
