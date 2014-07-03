var _ = require('underscore');
var domjs = require('domjs/lib/html5')(document);

/*!
 * \brief Build a confirmation button.
 *
 * \pre Called from within a domjs template.
 * \returns A domjs element.
 */
exports.confirmButton = function(text, callback) {
    var element = div();

    var firstTemplate = function() {
        element(
            button(
                { class: 'pure-button', onclick: buildSecond },
                _.isFunction(text)?text():text
                )
            );
    };
    var secondTemplate = function() {
        element(
            _.isFunction(text)?text():text,
            ': ',
            button(
                {
                    class: 'pure-button pure-button-primary',
                    onclick: (function() {
                        callback();
                        buildFirst();
                    }).bind(this)
                },
                'Yes'
                ),
            button({ class: 'pure-button', onclick: buildFirst }, 'No')
            );
    };
    var buildFirst = function() {
        element().innerHTML = '';
        domjs.build(firstTemplate);
    };
    var buildSecond = function() {
        element().innerHTML = '';
        domjs.build(secondTemplate);
    };

    buildFirst();

    return element;
};

/*!
 * Button that requests a confirmation from the user when clicked.
 *
 * \param text Text or function providing a template element displayed inside
 * the button.
 * \param callback Called with no arguments when the user clicks the button and
 * confirms their choice.
 */
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
                _.isFunction(this._text)?this._text():this._text
                )
            );
}

exports.ConfirmButton.prototype._buildFirst = function() {
    this._element().innerHTML = '';
    domjs.build(this._firstTemplate.bind(this));
}

exports.ConfirmButton.prototype._secondTemplate = function() {
    this._element(
            _.isFunction(this._text)?this._text():this._text,
            ': '
            )
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

