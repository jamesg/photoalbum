var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var LocationList      = require('./ui/locationlist').LocationList;
var Collapsable       = require('./ui/collapsable').Collapsable;
var PhotographList    = require('./ui/photograph/list').PhotographList;

exports.Locations = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Locations.prototype.element = function() {
    return this._element;
}

exports.Locations.prototype._template = function() {
    this._element = div();

    // Create the location list.
    var locationListCollapsable = new Collapsable('Locations', null);
    this._element(locationListCollapsable.element());
    var locationList = new LocationList(
            function(location) {
                api.photographsWithLocation(
                    location,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    api.locations(
            function(err, locations) {
                locationList.setLocationList( util.values(
                        util.mapValues(
                            locations,
                            function(location) { return location.location; }
                            )
                        ) );
            }
            );
    locationListCollapsable.setContent(locationList.element());

    // Create the photograph list.
    var photographListCollapsable = new Collapsable('Photographs', null);
    this._element(photographListCollapsable.element());

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);
    photographListCollapsable.setContent(photographList.element());
}

