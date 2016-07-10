<?php

/*
 * $site_path is made available by Drupal core. Perhaps there is a better way.
 */
$sites_subdir = basename($site_path);

$databases['default']['default'] = array (
    'database' => $sites_subdir,
    'username' => 'root',
    'password' => '',
    'prefix' => '',
    'host' => '127.0.0.1',
    'port' => '33067',
    'namespace' => 'Drupal\\Core\\Database\\Driver\\mysql',
    'driver' => 'mysql',
);

$settings['install_profile'] = 'minimal';
$config_directories['sync'] = '../config/'. $sites_subdir . '/sync';
