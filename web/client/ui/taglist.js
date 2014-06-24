var _ = require('underscore');
var domjs = require('domjs/lib/html5')(document);

var api = require('../api');

/*
 * Tags is a list of the form:
 * [ tag ]
 *
 * Callback is called with the id of the album selected.
 */
exports.TagList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.TagList.prototype.setTagList = function(tags) {
    if(!_.isArray(tags)) return;
    this._element().innerHTML = '';
    domjs.build(
        this._listTemplate.bind(this, tags)
        );
}

exports.TagList.prototype.element = function() {
    return this._element;
}

exports.TagList.prototype._listTemplate = function(tags) {
    for(l in tags) {
        var _li = li(
                {
                    onclick: this._callback.bind(this, tags[l].tag)
                },
                tags[l].tag
                );
        this._element(_li);
    }
}

exports.TagList.prototype._template = function() {
    this._element = ul({ 'class': 'albumlist' });
    this._element();
}

