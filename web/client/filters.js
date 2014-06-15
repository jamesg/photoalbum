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
    var filters = new PhotographFilter(
            function(apiFunction) {
                console.log('apiFunction ' + apiFunction);
                api.passThrough(
                    apiFunction,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    var filtersCollapsable = div(filters.element());

    var photographList = new PhotographList(function() {});
    var photographListCollapsable = div();

    photographList.setList([]);
    photographListCollapsable(photographList.element());

    this._element = div(
        filtersCollapsable,
        photographListCollapsable
        );
}

