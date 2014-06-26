var domjs = require('domjs/lib/html5')(document);

var api  = require('../../api');
var util = require('../../util');

var PhotographLi = require('./li').PhotographLi;

/*
 * Photographs is a list of the form:
 * [
 *  {
 *   'photograph_id': id,
 *   'title': title,
 *   'caption': caption,
 *   'taken': date taken,
 *   'location': location
 *  }
 * ]
 */
exports.PhotographList = function(callback) {
    this._callback = callback;

    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.PhotographList.prototype.element = function() {
    return this._element;
}

exports.PhotographList.prototype.setList = function(list) {
    this._element().innerHTML = '';
    this._element(
            domjs.build(
                this._listTemplate.bind(this, list)
                )
            );
}

exports.PhotographList.prototype._listTemplate = function(photographs) {
    for(photo in photographs) {
        var listItem = new PhotographLi(photographs[photo]);
        this._element(listItem.element());
    }
}

exports.PhotographList.prototype._template = function(photographs) {
    this._element = ul({ 'class': 'photographlist' });
    this._element();
}

