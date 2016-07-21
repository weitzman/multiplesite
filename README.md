Overview
============
An experiment in how to to handle the "multiplesite" pattern in Drupal 8. This is when you run dozens or hundreds of very similar sites. You want these sites to vary configuration in slight ways, but still easily push out config changes to all. Examples include [one site for each member of Congress](http://buytaert.net/us-house-of-representatives-using-drupal), or [one site for each musician in our portfolio](http://www.warnerbrosrecords.com/artists).

Getting Started
==============
1. Clone this repo
1. Run `composer install`
1. Run `cd web`
1. See [web/sites/settings.allsites.php](https://github.com/weitzman/multiplesite/blob/master/web/sites/settings.allsites.php). The DB port is configured for Acquia Dev Desktop. If needed, override that by editing this file or change settings.local.php in each settings subdir.
1. Run `drush @master site-install -vy --config-dir=../config/master/sync`. Do same for alpha and bravo sites, replacing alias name and dir name.
1. Verify that sites are working: `drush @master status`, `drush @alpha status`, `drush @bravo status`
1. In 3 new terminal windows, run `drush @master runserver`, `drush @alpha runserver`, `drush @bravo runserver`. This will give you 3 web sites to play with. Drush reports back the URL of the site.

You now have 3 working Drupal sites, mapped to the right databases and config dirs (see web/sites/settings.allsites.php).

Implementation Discussion
=============
There are two git repos:

1. [multiplesite](https://github.com/weitzman/multiplesite). This repo carries the shared for code for all the sites, and the "master" config. This where pull requests happen for new features and 99% of bug fixes. The only exception would be bug fixes that require site-specific configuration changes.
1. [multiplesite-config](https://github.com/weitzman/multiplesite-config). This repo has a subtree split of the master config in its master branch (see its README.md). Then we create branches off of master - one for each client site. [These branches are pulled into a subdirectory of /config during `composer install`](https://github.com/weitzman/multiplesite/blob/master/composer.json#L29).

We setup a Drupal multisite where the 'master' site carries the 'master' config, and the client sites merge in master config periodically. So the workflow is that client sites occasionally change config on their own sites and that config gets exported and committed to their own branch frequently. When the master wants to push out new config, we merge from multisite-config/master (or a tag there) into each client branch.

1. The 'minimal' profile must be used here as its a requirement of drush site-install --config-dir. One could likely do this with the config_installer profile but a custom profile is not likely to work. A custom distro should not be needed since client can just build upon minimal.
1. The 'default' site is deliberately unused. One must specify a site alias for all commands. See /drush/aliases.drushrc.php.
1. The `drush use` command is convenient when sending multiple Drush requests. `drush init` will customize your shell prompt so the current Drupal Site is shown.
1. This experiment uses Drupal multisite is used for convenience only. This technique works with separate docroots as well.

Adding a new Site (e.g. foo)
===================
1. Create settings subdir: `cp -r sites/alpha sites/foo`. No customization is needed in settings.php.
1. Add line in composer.json: `composer require multiplesite-config/foo dev-foo`
1. Create branch in multisite-config repo: `git checkout -b foo master && git push`
1. `composer update`
1. drush @foo site-install -vy --config-dir=../config/foo/sync

Findings
=============
1. A client can create a config entity whose machine name later conflicts with a name from master. Don't think much can be done about this.
1. Some config entities have broad permissions (e.g. 'Administer image styles'). We might prevent deletion but add/edit is OK.
    1. Disallow delete and have clients disable instead. Implement hook_entity_access() to deny delete operation.
    1. @todo Most config entities don't declare status key so don't have Enable/Disable operations (e.g. Image styles). If we add those, we could simply hide disabled entities like Formats does, or provide a grouped UI like Views does (more work). See https://www.drupal.org/node/1926376. A start at addressing this is in the [ms module](https://github.com/weitzman/multiplesite/tree/master/web/modules/custom/ms/ms).
1. Features module appears to be a poor fit here. We want to allow clients to _partially_ vary their config entities indefinitely. Features allows you to revert config but no other way to benefit from future changes.
1. Admin pages list and load config entities without overrides so the override system is a poor place for storing client variations.
1. For our custom modules, we want to check-in config entities with UUIDs in the file (unlike core). That way client sites have predictable UUIDs. For core and contrib modules, its better if those get enabled via config-import than via UI since clients's config entities will get standard UUIDs. This approach works as long as we core doesn't implement https://www.drupal.org/node/2161149.

Minutia
================
1. When fixing bugs while using a client site, a developer can choose to push commits to master config or to client config as needed. Pushing to client config happens automatically since thats 'origin'. If dev wants to integrate changes into multiplesite, add a remote pointing to multiplesite and then push commits there.

```
    git remote add multiplesite https://github.com/weitzman/multiplesite.git
    git checkout -b multiplesite-master multiplesite/master
    git cherry-pick <COMMITS>
    git push
```