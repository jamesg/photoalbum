var domjs = require('domjs/lib/html5')(document);

var api = require('./api');

exports.ChangePassword = function() {
    this._documentFragment = domjs.build(this._template.bind(this));
};

exports.ChangePassword.prototype.element = function() {
    return this._element;
};

exports.ChangePassword.prototype._template = function() {
    this._element = div();

    var messageBox = div({ class: 'messagebox' });

    var currentPassword = input({ type: 'password' });
    var newPassword1 = input({ type: 'password' });
    var newPassword2 = input({ type: 'password' });

    var changePassword = function() {
        api.loggedInUser(function(err, user) {
            if(err)
            {
                messageBox('Error retrieving account details.');
                return;
            }
            user.password = newPassword1().value;
            api.updateUser(user, function(err, user) {
                if(err)
                    messageBox('Error updating account details.');
                else
                    messageBox('Updates account details.');
            });
        });
        return false;
    };

    this._element(
            form(
                { class: 'pure-form', onsubmit: changePassword },
                label(
                    'Current password',
                    currentPassword
                    ),
                label(
                    'New password',
                    newPassword1
                    ),
                label(
                    'Repeat',
                    newPassword2
                    ),
                input({ type: 'submit', value: 'Change Password' })
                )
            );
};

