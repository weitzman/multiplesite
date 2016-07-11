Overview
============
An experiment in how to to handle the "multiplesite" pattern in Drupal 8. This is when you run dozens or hundreds of very similar sites. You want these sites to vary configuration in slight ways, but still easily push out onfig changes to all.

Implementation
=============


Notes and Limitations
=============
- A client could create a config entity whose machine name later conflcits with a name from master. That sort of conflict can happen with content as well.
- If a client deletes a config entity, it would come back with next import. We would need a "disable" instead. Not sure if this is supported by all config entity types
- The 'minimal' profile must be used here as its a requirement of drush site-install --config-dir. One could likely do this with the config_installer profile but a custom profile is not likely to work. A custom distro should not be needed since client can just build upon minimal.
- The 'default' site is deliberately unused. One must specify a site alias for all commands. See /drush/aliases.drushrc.php.
- The drush site-set command is useful when working on site repeatedly. `drush init` will customize your shell prompt so the current Site is shown.

Credits
================
- This repo is based on https://github.com/drupal-composer/drupal-project.