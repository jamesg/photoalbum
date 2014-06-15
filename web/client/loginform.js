var domjs = require('domjs/lib/html5')(document);

var api = require('./api');

/*
 * Display a login form (username and password).  The callback is called when
 * the user has logged in successfully and gained an authentication token.
 */
exports.LoginForm = function(callback) {
    this._callback = callback;

    domjs.build(this._template.bind(this));
}

exports.LoginForm.prototype.element = function() {
    return this._element;
}

exports.LoginForm.prototype._template = function() {
    var inlineInput = function(input_, label_ ) {
        return div(
                { class: 'pure-control-group' },
                label({ for: input_().name }, label_),
                input_
                );
    };
    var login = function() {
        api.login(
                usernameInput().value,
                passwordInput().value,
                (function(err, result) {
                    if(err)
                    {
                        console.log('login error', err);
                    }
                    else
                    {
                        localStorage.setItem('token', result);
                        this._callback();
                    }
                }).bind(this)
                );
        return false;
    };
    var usernameInput = input({ name: 'username' });
    var passwordInput = input({ name: 'password', type: 'password' });
    this._element = div(
            { class: 'pure-g' },
            div({ class: 'pure-u-8-24' }),
            div(
                { class: 'pure-u-8-24' },
                form(
                    { class: 'pure-form pure-form-stacked', onsubmit: login.bind(this) },
                    inlineInput(usernameInput, 'Username'),
                    inlineInput(passwordInput, 'Password'),
                    input({ type: 'submit', class: 'pure-button pure-button-primary' })
                ),
                div({ class: 'pure-u-8-24' })
                )
            );
}

