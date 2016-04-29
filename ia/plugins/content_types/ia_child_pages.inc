<?php

/**
 * Plugins are described by creating a $plugin array which will be used
 * by the system that includes this file.
 */
$plugin = array(
    'title' => t('IA Child Pages'),
    'description' => t('Render a list of child pages'),
    // 'single' => TRUE means has no subtypes.
    'single' => TRUE,
    // Constructor.
    'content_types' => array('ia_child_pages_content_type'),
    // Name of a function which will render the block.
    'render callback' => 'ia_child_pages_content_type_render',
    // The default context.
    'defaults' => array(),
    'admin info' => 'ia_child_pages_admin_info',
    // This explicitly declares the config form. Without this line, the func would be
    // ctools_plugin_example_no_context_content_type_edit_form.
    'edit form' => 'ia_child_pages_content_type_edit_form',
    'category' => array(t('Widgets'), -9),
    // this example does not provide 'admin info', which would populate the
    // panels builder page preview.
);

/**
 * Run-time rendering of the body of the block.
 *
 * @param $subtype
 * @param $conf
 *   Configuration as done at admin time.
 * @param $args
 * @param $context
 *   Context - in this case we don't have any.
 *
 * @return
 *   An object with at least title and content members.
 */
function ia_child_pages_content_type_render($subtype, $conf, $args, $context) {

  $block = new stdClass();

  // find the top level menu item.
  $tree = menu_tree_page_data('menu-ia');

  $level = 0;
  $paths = array();
  // Go down the active trail as far as possible.
  while ($tree) {
    // Loop through the current level's items until we find one that is in trail.
    while ($item = array_shift($tree)) {
      if ($item['link']['in_active_trail']) {
        // If the item is in the active trail, we count a new level.
        $level++;
        $paths[] = $item['link']['link_path'];
        if (!empty($item['below'])) {
          // If more items are available, we continue down the tree.
          $tree = $item['below'];
          break;
        }
        // If we are at the end of the tree, our work here is done.
        break 2;
      }
    }
  }


  $menulevel = count(array_unique($paths));

  // Menu block parameters.
  $config = array(
    'menu_name' => 'menu-ia',
    'parent_mlid' => 0,
    'sort' => FALSE,
    'follow' => 1,
    'level' => $menulevel + 1,
    'depth' => 1,
    'expanded' => TRUE,
    'title_link' => FALSE,
    'delta' => 'childpages',
  );

  // Build the menu tree via menu block module.
  $menu_tree = menu_tree_build($config);


  $entities = array();
  if (!empty($menu_tree['content']['#content'])) {
    // Return the renderable tree.
    $children = element_children($menu_tree['content']['#content']);
    foreach ($children as $key => $value) {

      // attempt to load as a node
      if ($node = menu_get_object('node', 1, $menu_tree['content']['#content'][$value]['#href'])) {
        $entities[] = node_view($node,$conf['child_view_mode']);

      }

      // attempt to load as a taxonomy term
      if ($term = menu_get_object('taxonomy_term', 2, $menu_tree['content']['#content'][$value]['#href'])) {
        $entities[] = taxonomy_term_view($term,$conf['child_view_mode']);
      }
    }
  }

  foreach ($entities as &$entity) {
      $entity['#prefix'] = '<div class="col-sm-'. (12/$conf['columns']) .'">';
      $entity['#suffix'] = '</div>';
  }

  $form = array();
  $form['row'] = array(
    '#type' => 'container',
    '#attributes' => array(
      'class' => 'row'
    )
  );

  $form['row']['nodes'] = $entities;
  $block->content = $form;


  return $block;
}






/**
 * 'Edit form' callback for the content type.
 * This example just returns a form; validation and submission are standard drupal
 * Note that if we had not provided an entry for this in hook_content_types,
 * this could have had the default name
 * ctools_plugin_example_no_context_content_type_edit_form.
 *
 */
function ia_child_pages_content_type_edit_form($form, &$form_state) {
    $conf = $form_state['conf'];

    $form['columns'] = array(
        '#type' => 'select',
        '#title' => t('No of columns'),
        '#options' => array(
            '1' => t('One column'),
            '2' => t('Two columns'),
            '3' => t('Three columns'),
            '4' => t('Four columns'),
        ),
        '#default_value' => (isset($conf['columns'])) ? $conf['columns'] : 3,
        '#description' => t('Choose column count'),
    );



  $form['child_view_mode'] = array(
    '#title' => 'Select a view mode',
    '#type' => 'select',
    '#options' => _ia_child_view_modes(),
    '#default_value' => $conf['child_view_mode'],
  );

    // We dont want a title
//    $form['override_title']['#access'] = FALSE;
//    $form['override_title_text']['#access'] = FALSE;
//    $form['override_title_markup']['#access'] = FALSE;

    // return the renderable array.
    return $form;
}

/**
 * @param $form
 * @param $form_state
 */
function ia_child_pages_content_type_edit_form_submit(&$form, &$form_state) {
    $keys = array('child_view_mode', 'columns');

    foreach ($keys as $key) {
        $form_state['conf'][$key] = $form_state['values'][$key];
    }
}


/**
 * Ctools plugin function hook_admin_info.
 */
function ia_child_pages_admin_info($subtype, $conf, $context = NULL) {

    if (empty($output) || !is_object($output)) {
        $output = new stdClass();


        $output->title = 'Child pages';
        $output->content = t('No info available.');
    }

    return $output;
}



function _ia_child_view_modes() {

  return _ia_get_view_modes('node', 'node');

  $settings = variable_get('ia');

  $view_modes = array();
  foreach ($settings['general']['child']['view_modes'] as $key => $value) {

    if ($value === $key) {
      $view_modes[$key] = $key;
    }
  }

  return $view_modes;

}