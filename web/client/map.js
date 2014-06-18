var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var KmTileView  = require('./ui/kmtileview').KmTileView;
var MapSearch   = require('./ui/mapsearch').MapSearch;

exports.Map = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Map.prototype.element = function() {
    return this._element;
}

exports.Map.prototype._template = function() {
    var kmTileView = new KmTileView();
    var mapSearch = new MapSearch(kmTileView.loadKmRef.bind(kmTileView));

    this._element = div(
            { class: 'mainmenu pure-g' },
            div(
                { class: 'pure-u-1-1' },
                mapSearch.element()
               ),
            div(
                { class: 'pure-u-1-1' },
                kmTileView.element()
               )
            );
}

