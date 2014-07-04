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

