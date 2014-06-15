var domjs = require('domjs/lib/html5')(document);

var PhotographForm = require('./ui/photographform').PhotographForm;

exports.NewPhotograph = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.NewPhotograph.prototype.element = function() {
    return this._element;
}

exports.NewPhotograph.prototype._template = function() {
    this._element = div(
        (new PhotographForm()).element()
        );
}

