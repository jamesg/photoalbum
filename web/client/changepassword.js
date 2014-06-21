var domjs = require('domjs/lib/html5')(document);

var api = require('./api');

var MessageBox = require('./ui/messagebox').MessageBox;

exports.ChangePassword = function() {
    this._documentFragment = domjs.build(this._template.bind(this));
};

exports.ChangePassword.prototype.element = function() {
    return this._element;
};

exports.ChangePassword.prototype._template = function() {
    this._element = div();

    this._messageBox = new MessageBox();

    var currentPassword = input({ type: 'password' });
    var newPassword1 = input({ type: 'password' });
    var newPassword2 = input({ type: 'password' });

    var changePassword = (function() {
        api.loggedInUser((function(err, user) {
            if(err)
            {
                this._messageBox.displayError('Error retrieving account details.');
                return;
            }
            // TODO: don't rely on the password being present.
            if(currentPassword().value != user.password)
            {
                this._messageBox.displayError('Current password doesn\'t match');
                return;
            }
            user.password = newPassword1().value;
            api.updateUser(user, (function(err, user) {
                if(err)
                    this._messageBox.displayError(err);
                else
                    this._messageBox.displaySuccess('Updated account details.');
            }).bind(this));
        }).bind(this));
        return false;
    }).bind(this);

    this._element(
            this._messageBox.element(),
            form(
                {
                    class: 'pure-form pure-form-stacked',
                    onsubmit: changePassword.bind(this)
                },
                div(
                    { class: 'pure-control-group' },
                    label(
                        'Current password',
                        currentPassword
                        )
                   ),
                div(
                    { class: 'pure-control-group' },
                    label(
                        'New password',
                        newPassword1
                        )
                    ),
                div(
                    { class: 'pure-control-group' },
                    label(
                        'Repeat',
                        newPassword2
                        )
                   ),
                input(
                        {
                            type: 'submit',
                            class: 'pure-button',
                            value: 'Change Password'
                        }
                        )
                )
            );
};

