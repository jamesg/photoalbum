var domjs = require('domjs/lib/html5')(document);

var api  = require('../../api');
var util = require('../../util');

exports.PhotographView = function(photographId, callback) {
    this._callback = callback;
    this._photographId = photographId;
    this._documentFragment = domjs.build(this._template.bind(this));
}

exports.PhotographView.prototype.element = function() {
    return this._element;
}

exports.PhotographView.prototype._template = function() {
    this._element = img(
        {
            class: 'pure-img view-image',
            src: '/jpeg_image' +
                util.queryString(
                    {
                        token: localStorage['token'],
                        photograph_id: this._photographId,
                        width: 800,
                        height: 600
                    }
                    ),
            onclick: this._callback
        }
        );
}

