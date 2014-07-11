var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

var icon = require('./icon').icon;

exports.MainMenu = function(application) {
    this._application = application;

    this._documentFragment = domjs.build(this._template.bind(this));

    api.features(
            (function(err, features) {
                domjs.build(this._buttonsTemplate.bind(this, features));
            }).bind(this)
            );
}

exports.MainMenu.prototype.element = function() {
    return this._element;
}

exports.MainMenu.prototype._template = function() {
    this._element = div({ class: 'mainmenu content-view pure-g' });
}

exports.MainMenu.prototype._buttonTemplate = function(icon_, text, callback) {
    return button(
            { 'onclick': callback, class: 'pure-button' },
            icon(icon_),
            br(),
            text
            );
}

exports.MainMenu.prototype._sectionTemplate = function(icon_, title, subtitle, buttons) {
    return div(
            { class: 'pure-u-1-1 mainmenu-section' },
            h2(icon(icon_), title, ' ', small(subtitle)),
            buttons
            );
}

exports.MainMenu.prototype._buttonsTemplate = function(features) {
    if(features['photographs']) {
        this._element(
            this._sectionTemplate(
                'layers',
                'Photographs',
                'Browse and upload photographs.',
                [
                this._buttonTemplate(
                    'image',
                    'Albums',
                    this._application.showAlbums.bind(this._application)
                    ),
                this._buttonTemplate(
                    'pin',
                    'Locations',
                    this._application.showLocations.bind(this._application)
                    ),
                this._buttonTemplate(
                    'list',
                    'Tags',
                    this._application.showTags.bind(this._application)
                    ),
                this._buttonTemplate(
                    'clock',
                    'Filters',
                    this._application.showFilters.bind(this._application)
                    ),
                this._buttonTemplate(
                    'cloud-upload',
                    'Upload',
                    this._application.showNewPhotograph.bind(this._application)
                    ),
                this._buttonTemplate(
                    'graph',
                    'Statistics',
                    this._application.showStatistics.bind(this._application)
                    )
                ]
                ),
            this._sectionTemplate(
                'pencil',
                'Notes',
                'Read and edit textual notes.',
                [
                this._buttonTemplate(
                    'text',
                    'Notes',
                    this._application.showNotes.bind(this._application)
                    ),
                this._buttonTemplate(
                    'pencil',
                    'Note Editor',
                    this._application.showNoteEditor.bind(this._application)
                    )
                ]
                )
            );
    }
    if(features['gazetteer']) {
        this._element(
            this._sectionTemplate(
                'globe',
                'Maps',
                'Search for Ordnance Survey maps.',
                [
                this._buttonTemplate(
                    'map',
                    'Gazetteer',
                    this._application.showMap.bind(this._application)
                    )
                ]
                )
            );
    }
    this._element(
            this._sectionTemplate(
                'person',
                'Account',
                'Manage your user account.',
                [
                this._buttonTemplate(
                    'key',
                    'Change Password',
                    this._application.changePassword.bind(this._application)
                    ),
                this._buttonTemplate(
                    'account-logout',
                    'Log Out',
                    this._application.logout.bind(this._application)
                    )
                ]
                )
            );
}

