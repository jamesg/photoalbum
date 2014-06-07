var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

/*
 * Locations is a list of the form:
 * [ location ]
 *
 * Callback is called with the id of the album selected.
 */
exports.LocationList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.LocationList.prototype.setLocationList = function(locations) {
    this._element().innerHTML = '';
    this._element(
            domjs.build(
                this._listTemplate.bind(this, locations)
                )
            );
}

exports.LocationList.prototype.element = function() {
    return this._element;
}

exports.LocationList.prototype._listTemplate = function(locations) {
    for(l in locations) {
        var _li = li(
                {
                    onclick: this._callback.bind(this, locations[l])
                },
                locations[l]
                );
        this._element(_li);
    }
}

/*
 * Template for the location list.
 */
exports.LocationList.prototype._template = function() {
    this._element = ul({ 'class': 'albumlist' });
    this._element();
}

