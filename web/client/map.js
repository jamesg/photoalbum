var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var Collapsable = require('./ui/collapsable').Collapsable;
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
    this._element = div();

    var mapSearchCollapsable = new Collapsable(
            'Search',
            (new MapSearch(
                function(kmRef) {
                    console.log('km_ref: ' + kmRef);
                    kmTileView.loadKmRef(kmRef);
                }
                )).element()
            );
    this._element(mapSearchCollapsable.element());

    var kmTileView = new KmTileView();
    var kmTileViewCollapsable = new Collapsable('Tile View', kmTileView.element());
    this._element(kmTileViewCollapsable.element());
}

