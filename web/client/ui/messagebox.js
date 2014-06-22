var domjs = require('domjs/lib/html5')(document);

/*
 * Construct a message box for displaying status messages to the user.
 *
 * A message box is initially empty.  Multiple messages can be added using the
 * display(Info|Success|Warning|Error) functions.  Each message will have a
 * close button that can be used to dismiss the message.
 */
exports.MessageBox = function() {
    domjs.build(template.bind(this));

    this.element = function() {
        return this._element;
    };

    this.displayInfo    = displayMessage.bind(this, 'info');
    this.displaySuccess = displayMessage.bind(this, 'success');
    this.displayWarning = displayMessage.bind(this, 'warning');
    this.displayError   = displayMessage.bind(this, 'error');
};

/*
 * Display the inner message in its own box with a close button.
 */
var innerMessage = function(style, message) {
    var element;

    /*
     * Remove this message from the list of messages - actually, the
     * messagebox-empty class just hides the element.  Removing it from the DOM
     * is more difficult.
     */
    var hide = function() {
        domjs.build(function() {
            element({ class: 'messagebox messagebox-empty' });
            element().innerHTML = '';
        });
    }

    var template = function() {
        element = div(
                { class: 'messagebox messagebox-' + style },
                span(
                    message,
                    button({ class: 'pure-button', onclick: hide }, 'Close')
                    )
                );
    };

    domjs.build(template);

    return element;
};

var template = function() {
    this._element = div();
};

var displayMessage = function(level, message) {
    this._message = message;
    this._element(innerMessage(level, message));
};

