var domjs = require('domjs/lib/html5')(document);

var api  = require('../api');
var util = require('../util');

exports.Statistics = function() {
    this._documentFragment = domjs.build(this._template.bind(this));
    this.reload();
}

exports.Statistics.prototype.reload = function() {
    api.statistics(
            (function(err, statistics) {
                this._statistics = statistics;
                domjs.build(this._detailTemplate.bind(this));
            }).bind(this)
            );
}

exports.Statistics.prototype._template = function() {
    this._element = table();
}

exports.Statistics.prototype._detailTemplate = function() {
    this._element(
            tr(
                td('Photograph Count'), td(this._statistics.total_count)
              )
         );
}

exports.Statistics.prototype.element = function() {
    return this._element;
}

