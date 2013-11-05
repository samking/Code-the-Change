<?php

/**
 * @file
 * Default theme implementation to configure blocks.
 *
 * Available variables:
 * - $mimes: An array of regions. Keyed by name with the title as value.
 * - $perms_listing: An array of blocks keyed by region and then delta.
 * - $form_extra: left over form parts.
 *
 * Each $perms_listing[$region] contains an array of blocks for that region.
 *
 * Each $data in $perms_listing[$region] contains:
 * - $data->region_title: Region title for the listed block.
 * - $data->block_title: Block title.
 * - $data->region_select: Drop-down menu for assigning a region.
 * - $data->weight_select: Drop-down menu for setting weights.
 * - $data->configure_link: Block configuration link.
 * - $data->delete_link: For deleting user added blocks.
 *
 * @see template_preprocess_block_admin_display_form()
 * @see theme_block_admin_display()
 */
?>
<?php
  foreach ($mimes as $region => $title) {
    drupal_add_tabledrag('te_perms', 'match', 'sibling', 'perm-mime-select', 'perm-mime-' . $region, NULL, FALSE);
    drupal_add_tabledrag('te_perms', 'order', 'sibling', 'perm-weight', 'perm-weight-' . $region);
  }

?>
<table id="te_perms">
  <thead>
    <tr>
      <th><?php print t('Mime Type'); ?></th>
      <th><?php print t('Order'); ?></th>
      <th><?php print t('Operations'); ?></th>
      <th><?php print t('Access Check Type'); ?></th>
      <th><?php print t('Access Arguments'); ?></th>
      <th><?php print t('Actions'); ?></th>
    </tr>
  </thead>
  <tbody>
    <?php $row = 0; ?>
    <?php foreach ($mimes as $region => $title): ?>
      <tr class="region-title region-title-<?php print $region?>">
        <td colspan="7"><?php print $title; ?></td>
      </tr>
      <tr class="region-message region-<?php print $region?>-message <?php print empty($perms_listing[$region]) ? 'region-empty' : 'region-populated'; ?>">
        <td colspan="7"><em><?php print t('No permissions for this file type'); ?></em></td>
      </tr>
      <?php foreach ($perms_listing[$region] as $delta => $data): ?>
      <tr class="draggable <?php print $row % 2 == 0 ? 'odd' : 'even'; ?>">
        <td class="perm"><?php print $data->mime; ?></td>
        <td><?php print $data->weight; ?></td>
        <td><?php print $data->ops; ?></td>
        <td><?php print $data->check; ?></td>
        <td><?php print $data->args; ?></td>
        <td><?php print $data->action; ?>
          <?php if (isset($data->global)):?>
            <div class="description"><strong><?php print $data->global; ?></strong></div>
          <?php endif; ?>
        </td>
      </tr>
      <?php $row++; ?>
      <?php endforeach; ?>
    <?php endforeach; ?>
  </tbody>
</table>

<?php print $form_extra; ?>
