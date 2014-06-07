var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

// Ui
var Collapsable    = require('./ui/collapsable').Collapsable;
var StatisticsView = require('./ui/statistics').Statistics;

exports.Statistics = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Statistics.prototype.element = function() {
    return this._element;
}

exports.Statistics.prototype._template = function() {
    this._element = div();

    var statisticsCollapsable = new Collapsable('Statistics', null);
    this._element(statisticsCollapsable.element());
    var statistics = new StatisticsView();
    statisticsCollapsable.setContent(statistics.element());
}

