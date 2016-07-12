Overview
============
An experiment in how to to handle the "multiplesite" pattern in Drupal 8. This is when you run dozens or hundreds of very similar sites. You want these sites to vary configuration in slight ways, but still easily push out config changes to all. Examples include [one site for each member of Congress](http://buytaert.net/us-house-of-representatives-using-drupal), or [one site for each musician in our portfolio](http://www.warnerbrosrecords.com/artists).

Getting Started
==============
1. Clone this repo
1. Run `composer install`
1. Run `cd web`
1. Run `drush msi -vy`. This creates a top level 'config' directory and one subdirectory for each site in the Drupal multisite (currently 3).
1. view web/sites/settings.allsites.php. The DB is setup for Acquia Dev Desktop. If needed, override that by creating a settings.local.php in each settings subdir.
1. Run `drush @master site-install -vy --config-dir=../config/master/sync`. Do same for alpha and bravo sites, replacing alias name and dir name.
1. Verify that sites are working: `drush @master status`, `drush @alpha status`, `drush @bravo status`
1. In 3 new terminal windows, run `drush @master runserver`, `drush @alpha runserver`, `drush @bravo runserver`. This will give you 3 web sites to play with. Drush reports back the URL of the site.

You now have 3 working Drupal sites, mapped to the right databases and config dirs (see web/sites/settings.allsites.php).

Implementation Discussion
=============
There are two git repos:

1. [multiplesite](https://github.com/weitzman/multiplesite). This repo carries the shared for code for all the sites.
1. [multiplesite-config](https://github.com/weitzman/multiplesite-config). This repo has a master branch, where the "golden" Drupal config is stored. Then we create branches off of master - one for each client site. These branches are cloned into place under a /config directory by the `drush msi` command. This build step seems cleaner than a submodule approach which would embed config repos into the code repo.

We setup a Drupal multisite where the 'master' site carries the 'golden' config, and the client sites merge in golden config periodically. So the workflow is that client sites occasionally change config on their own sites and that config gets exported and committed to their own branch frequently. When the master wants to push out new config, we merge from multisite-config/master (or a tag there) into each client branch.

1. The 'minimal' profile must be used here as its a requirement of drush site-install --config-dir. One could likely do this with the config_installer profile but a custom profile is not likely to work. A custom distro should not be needed since client can just build upon minimal.
1. The 'default' site is deliberately unused. One must specify a site alias for all commands. See /drush/aliases.drushrc.php.
1. The `drush use` command is convenient when sending multiple Drush requests. `drush init` will customize your shell prompt so the current Drupal Site is shown.
1. This experiment uses Drupal multisite is used for convenience only. This technique works with separate docroots as well.

Findings
=============
1. A client can create a config entity whose machine name later conflicts with a name from master. Maybe auto-prefix client-made config entities (e.g. bravo-image-style-hero). @todo Investigate \Drupal\Core\Render\Element\MachineName.
1. Some config entities have broad permissions (e.g. 'Administer image styles'). We might not want them to delete but add/edit is OK.
    1. Disallow delete and have clients disable instead. Implement hook_entity_access() to deny delete operation.
    1. @todo Most config entities don't declare status key so don't have Enable/Disable operations (e.g. Image styles). If we add those, we could simply hide disabled entities like Formats does, or provide a segregated UI like Views does (more work). See https://www.drupal.org/node/1926376. A start at addressing this is in the [ms module](https://github.com/weitzman/multiplesite/tree/master/web/modules/custom/ms/ms).
1. It is possible to have git conflicts when merging from master to client repo. There may be a way with [rerere](https://medium.com/@porteneuve/fix-conflicts-only-once-with-git-rerere-7d116b2cec67#.cofpprewi) to save the conflict resolution for use on other client branches.


Credits
================
- This repo is based on https://github.com/drupal-composer/drupal-project.