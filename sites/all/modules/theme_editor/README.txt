Theme Editor provides a UI for editing themes installed on your Drupal site
The current release supports the following features

- File versioning
- Adding, uploading, and deleteing files from themes
- Enabling/disabling editor changes for themes
- Views integration

Expected features include

- Syntax highlighting
- Image editor/replacement for images
- File diff for versions
- Access control on files similiar to node access/grants
- More fine grained control for themes/files options
- Tighter views integration.


***************************
*      Installation       *
***************************

In order for Theme Editor to work you will need a writeable directory within in your file structure.

You DO NOT need to make the actuall theme folders writeable.

A prefered spot is something like 'sites/example.com/theme_editor'

Once that is set up go to 'Appearance > Settings> Theme Editor' and enter that directory

This directory must be owned by the server user for writing and chmod'ing.

DO NOT USE A DIRECTORY WITHIN THE PUBLIC FILESYSTEM FOLDER i.e. 'sites/example.com/files/theme_editor'
as this may create a security hole where unauthorized users could access your theme files.