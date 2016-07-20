<?php

function multiplesite_drush_command() {
  $items['multiplesite-init'] = array(
    'description' => 'Create the client config directories via git clones.',
    'arguments' => array(),
    'options' => array(),
    'aliases' => array('msi'),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_ROOT,
  );
  return $items;
}

/*
 * Command callback.
 */
function drush_multiplesite_init() {
  $config = dirname(drush_get_context('DRUSH_DRUPAL_ROOT')) . '/config';

  // Wipe all but 'master' subdir.
  $scandir = @scandir($config);
  foreach ($scandir as $item) {
    if ($item == '.' || $item == '..' || $item == 'master') {
      continue;
    }
    drush_delete_dir($config. '/'. $item);
  }

  // Place all client subdirs into /config.
  foreach (multiplesite_get_sites() as $site) {
    $exec = "git clone --branch %s https://github.com/weitzman/multiplesite-config.git %s";
    drush_shell_cd_and_exec($config, $exec, $site, $site);
  }
}

/*
 * Helper function to get list of sites.
 */
function multiplesite_get_sites() {
  return array('alpha', 'bravo');
}


