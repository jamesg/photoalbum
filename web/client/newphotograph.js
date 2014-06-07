var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

// Ui
var Collapsable    = require('./ui/collapsable').Collapsable;
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
    this._element = div();

    // Create the new photograph form.
    var newPhotographForm = new PhotographForm();
    var photographFormCollapsable = new Collapsable(
        'New Photograph',
        newPhotographForm.element()
        );

    this._element(photographFormCollapsable.element());
}

