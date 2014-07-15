var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var LocationList      = require('./ui/locationlist').LocationList;
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
    this._element = div({ class: 'pure-g padded' });

    // Create the location list.
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
    this._element(
            div(
                { class: 'content-view pure-u-1-1' },
                h2('Locations'),
                locationList.element()
                )
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

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    this._element(div({ class: 'pure-u-1-1' }, photographList.element()));
    photographList.setList([]);
}

