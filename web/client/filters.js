var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var PhotographFilter = require('./ui/photographfilter').PhotographFilter;
var Collapsable      = require('./ui/collapsable').Collapsable;
var PhotographList   = require('./ui/photographlist').PhotographList;

exports.Filters = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Filters.prototype.element = function() {
    return this._element;
}

exports.Filters.prototype._template = function() {
    this._element = div();

    var filtersCollapsable = new Collapsable('Filters', null);
    this._element(filtersCollapsable.element());
    var filters = new PhotographFilter(
            function(apiFunction, params) {
                console.log('apiFunction ' + apiFunction);
                api.passThrough(
                    apiFunction,
                    params,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    filtersCollapsable.setContent(filters.element());

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

