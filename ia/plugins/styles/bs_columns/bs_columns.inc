<?php

/**
 * @file
 * Definition of the 'default' panel style.
 */

// Plugin definition
$plugin = array(
  'title' => t('Bootstrap columns'),
  'description' => t('Bootstrap column layout'),
  'render region' => 'panels_bs_column_style_render_region',
  'settings form' => 'panels_bs_columns_style_settings_form',

);

function panels_bs_columns_style_settings_form($style_settings) {
  $form['columns'] = array(
    '#type' => 'select',
    '#title' => t('No of columns'),
    '#options' => array(
      '1' => t('One column'),
      '2' => t('Two columns'),
      '3' => t('Three columns'),
      '4' => t('Four columns'),
    ),
    '#default_value' => (isset($style_settings['columns'])) ? $style_settings['columns'] : 3,
    '#description' => t('Choose column count'),
  );
  $form['gutter'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove gutter'),
    '#default_value' => (isset($style_settings['gutter'])) ? $style_settings['gutter'] : 'no-gutter',
    '#description' => t('Remove outer gutter'),
  );

  $form['gutter_mobile'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove gutter on mobile only'),
    '#default_value' => (isset($style_settings['gutter_mobile'])) ? $style_settings['gutter_mobile'] : 'grid--no-gutters--mobile',
    '#description' => t('Remove outer gutter when viewing on mobile only.'),
  );

  $form['gutter_desktop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Remove gutter on desktop only'),
    '#default_value' => (isset($style_settings['gutter_desktop'])) ? $style_settings['gutter_desktop'] : 'grid--no-gutters--desktop',
    '#description' => t('Remove outer gutter when viewing on desktop only.'),
  );

  $form['style'] = array(
    '#type' => 'select',
    '#title' => t('Row style'),
    '#options' => array(
      'none' => t('None'),
      'grey-bg' => t('Grey background'),
    ),
    '#default_value' => (isset($style_settings['style'])) ? $style_settings['style'] : 'none',
  );
  return $form;
}

/**
 * Render callback.
 *
 * @ingroup themeable
 */
function theme_panels_bs_column_style_render_region($vars) {
  // @todo figure out a better way of getting path
  drupal_add_css(drupal_get_path('module', 'gel_ctools') . '/plugins/styles/bs_columns/bs_columns.css');

  $settings = $vars['settings'];
  $columns = empty($settings['columns']) ? 3 : $settings['columns'];
  $gutter = empty($settings['gutter']) ? '' : 'no-gutter';
  $gutter_mobile = empty($settings['gutter_mobile']) ? '' : 'grid--no-gutters--mobile';
  $gutter_desktop = empty($settings['gutter_desktop']) ? '' : 'grid--no-gutters--desktop';
  $style = (empty($settings['style']) || $settings['style'] == 'none') ? '' : $settings['style'];
  $chunks = array_chunk($vars['panes'], $columns);
  $bs_column_number = 12 / $columns;
  $output = '';
  foreach ($chunks as $panes) {

    $classes = trim('row '.$gutter. ' ' . $gutter_mobile . ' ' . $gutter_desktop . ' ' .$style);
    $output .= '<div class="' . $classes . '">';
    foreach ($panes as $pane) {
      $output .= '<div class="col-md-' . $bs_column_number . '">';
      $output .= $pane;
      $output .= '</div>';
    }
    $output .= '</div>';
  }

  return $output;
}

