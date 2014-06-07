var domjs = require('domjs/lib/html5')(document);

var util = require('../util');

exports.PhotographFilter = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(this._template.bind(this));
}

exports.PhotographFilter.prototype.element = function() {
    return this._element;
}

exports.PhotographFilter.prototype._template = function() {
    this._element = ul({ 'class': 'filterlist' });

    this._element(li(
                {
                    onclick: this._callback.bind(
                                 this, 'uncategorised_photographs', []
                                 )
                },
                'Photographs Without Album'
                ));
    this._element(li(
                {
                    onclick: this._callback.bind(
                                 this, 'recent_photographs', []
                                 )
                },
                'Recently Taken'
                ));
}

