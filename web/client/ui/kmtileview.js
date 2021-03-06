var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

exports.KmTileView = function() {
    this._documentFragment = domjs.build(this._template.bind(this));
}

exports.KmTileView.prototype.element = function() {
    return this._element;
}

exports.KmTileView.prototype._template = function() {
    this._element = div();
}

var tileImageUrl = function(kmRef) {
    return '/map_tile_km' + util.queryString(
            {
                token: localStorage['token'],
                region: kmRef.substring(0, 2).toLowerCase(),
                eastings: kmRef.substring(2, 4),
                northings: kmRef.substring(4, 6)
            }
            );
}

exports.KmTileView.prototype._mapTemplate = function() {
    this._element().innerHTML = '';
    this._element(
            div(
                { class: 'pure-g' },
                div({ class: 'pure-u-1-5' }),
                div(
                    { class: 'pure-u-3-5' },
                    img(
                        {
                            style: 'max-width:100%',
                            alt: this._kmRef,
                            src: tileImageUrl(this._kmRef)
                        }
                        )
                   ),
                div({ class: 'pure-u-1-5' })
                )
            );
}

exports.KmTileView.prototype.loadKmRef = function(kmRef) {
    this._kmRef = kmRef;
    domjs.build(this._mapTemplate.bind(this));
}

