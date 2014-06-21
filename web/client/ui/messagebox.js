var domjs = require('domjs/lib/html5')(document);

var template = function() {
    this._element = div({ class: 'messagebox messagebox-empty' });
};

var innerTemplate = function(style) {
    if(this._message)
        this._element(
                { class: 'messagebox messagebox-' + style },
                this._message
                );
};

var displayMessage = function(level, message) {
    this._message = message;
    this._element().innerHTML = '';
    domjs.build(innerTemplate.bind(this, level));
};

exports.MessageBox = function() {
    domjs.build(template.bind(this));

    this.element = function() {
        return this._element;
    };

    this.displayInfo = displayMessage.bind(this, 'info');
    this.displaySuccess = displayMessage.bind(this, 'success');
    this.displayWarning = displayMessage.bind(this, 'warning');
    this.displayError = displayMessage.bind(this, 'error');
};

