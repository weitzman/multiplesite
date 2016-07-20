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

  foreach (multiplesite_get_sites() as $site) {
    // Place each client subdir into /config.
    $exec = "git clone --branch %s https://github.com/weitzman/multiplesite-config.git %s";
    drush_shell_cd_and_exec($config, $exec, $site, $site);

    // Add a remote to multiplesite for cherry-picking commits to there from client site.
    $exec = "git remote add git multiplesite https://github.com/weitzman/multiplesite.git";
    drush_shell_cd_and_exec($config, $exec);

    // Fetch files/branches from remote so that user is not confused when they don't exist.
    $exec = "git fetch --all";
    drush_shell_cd_and_exec($config, $exec);
  }
}

/*
 * Helper function to get list of sites.
 */
function multiplesite_get_sites() {
  return array('alpha', 'bravo');
}


