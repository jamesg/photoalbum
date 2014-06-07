var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

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
    this._element = ul({ 'class': 'mainmenu' });
}

exports.MainMenu.prototype._buttonsTemplate = function(features) {
    if(features['photographs']) {
        this._element(
                this._buttonTemplate(
                    'Albums',
                    this._application.showAlbums.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Locations',
                    this._application.showLocations.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Tags',
                    this._application.showTags.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Filters',
                    this._application.showFilters.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'New Photograph',
                    this._application.showNewPhotograph.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Statistics',
                    this._application.showStatistics.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Notes',
                    this._application.showNotes.bind(this._application)
                    )
                );
        this._element(
                this._buttonTemplate(
                    'Note Editor',
                    this._application.showNoteEditor.bind(this._application)
                    )
                );
    }
    if(features['gazetteer']) {
        this._element(
                this._buttonTemplate(
                    'Gazetteer',
                    this._application.showMap.bind(this._application)
                    )
                );
    }
}

exports.MainMenu.prototype._buttonTemplate = function(text, callback) {
    return li(button({ 'onclick': callback }, text));
}

