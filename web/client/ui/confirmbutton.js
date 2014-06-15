var domjs = require('domjs/lib/html5')(document);

exports.ConfirmButton = function(text, callback) {
    this._text = text;
    this._callback = callback;

    domjs.build(this._template.bind(this));

    this._buildFirst();
}

exports.ConfirmButton.prototype._template = function() {
    this._element = span();
}

exports.ConfirmButton.prototype._firstTemplate = function() {
    this._element(
            button(
                {
                    class: 'pure-button',
                    onclick: this._buildSecond.bind(this)
                },
                this._text
                )
            );
}

exports.ConfirmButton.prototype._buildFirst = function() {
    this._element().innerHTML = '';
    domjs.build(this._firstTemplate.bind(this));
}

exports.ConfirmButton.prototype._secondTemplate = function() {
    this._element(this._text + ': ');
    this._element(
            button(
                {
                    class: 'pure-button pure-button-primary',
                    onclick: (function() {
                        this._callback();
                        this._buildFirst();
                    }).bind(this)
                },
                'Yes'
                )
            );
    this._element(
            button(
                {
                    class: 'pure-button',
                    onclick: this._buildFirst.bind(this)
                },
                'No'
                )
            );
}

exports.ConfirmButton.prototype._buildSecond = function() {
    this._element().innerHTML = '';
    domjs.build(this._secondTemplate.bind(this));
}

exports.ConfirmButton.prototype.element = function() {
    return this._element;
}

