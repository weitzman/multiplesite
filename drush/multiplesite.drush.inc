<?php

function multiplesite_drush_command() {
  $items['multiplesite-init'] = array(
    'description' => 'Create the config directories via git clones.',
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
  if (file_exists($config)) {
    if (!drush_confirm('Are you sure you want to delete and re-install the config directories?')) {
      return drush_user_abort();
    }
    else {
      drush_delete_dir($config, TRUE);
    }
  }
  drush_mkdir($config);
  foreach (multiplesites_get_sites() as $site) {
    $exec = "git clone --branch %s https://github.com/weitzman/multiplesite-config.git %s";
    drush_shell_cd_and_exec($config, $exec, $site, $site);
  }
}

/*
 * Helper function to get list of sites.
 */
function multiplesite_get_sites() {
  return array('master', 'alpha', 'bravo');
}


