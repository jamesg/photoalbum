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
    this._element = div({ class: 'pure-g' });

    var filtersCollapsable = div({ class: 'pure-u-1-5' });//new Collapsable('Filters', null);
    this._element(filtersCollapsable);
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
    filtersCollapsable(filters.element());

    // Create the photograph list.
    var photographListCollapsable = div({ class: 'pure-u-4-5' });//new Collapsable('Photographs', null);
    this._element(photographListCollapsable);

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);
    photographListCollapsable(photographList.element());
}

