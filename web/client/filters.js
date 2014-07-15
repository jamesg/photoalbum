var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var PhotographFilter = require('./ui/photograph/filter').PhotographFilter;
var Collapsable      = require('./ui/collapsable').Collapsable;
var PhotographList   = require('./ui/photograph/list').PhotographList;

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
                api.passThrough(
                    apiFunction,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );

    var photographList = new PhotographList(function() {});
    photographList.setList([]);

    this._element = div(
        filters.element(),
        photographList.element()
        );
}

