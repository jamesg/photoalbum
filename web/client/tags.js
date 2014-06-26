var domjs = require('domjs/lib/html5')(document);

var api = require('./api');
var util = require('./util');

var TagList        = require('./ui/taglist').TagList;
var Collapsable    = require('./ui/collapsable').Collapsable;
var PhotographList = require('./ui/photograph/list').PhotographList;

exports.Tags = function() {
    this._documentFragment = domjs.build(
            this._template.bind(this)
            );
}

exports.Tags.prototype.element = function() {
    return this._element;
}

exports.Tags.prototype._template = function() {
    this._element = div();

    this._element(
        ul(
            {
                class: 'pure-menu pure-menu-open pure-menu-horizontal'
            },
            li(a(
                {
                    onclick: function() {
                        api.tagsAlphabetical(function(err, tags) {
                            if(err)
                                console.log('retrieving alphabetical tags', err);
                            else
                                tagList.setTagList(tags);
                        }
                        );
                    }
                },
                'Order Alphabetically'
              )),
            li(a(
                {
                    onclick: function() {
                        api.tagsPopular(function(err, tags) {
                            if(err)
                                console.log('retrieving popular tags', err);
                            else
                                tagList.setTagList(tags);
                        }
                        );
                    }
                },
                'Order by Popularity'
                ))
          )
        );

    // Create the tag list.
    var tagListCollapsable = new Collapsable('Tags', null);
    this._element(tagListCollapsable.element());
    var tagList = new TagList(
            function(tag) {
                api.photographsWithTag(
                    tag,
                    function(err, list) {
                        photographList.setList(list);
                    }
                    );
            }
            );
    api.tags(
            function(err, tags) {
                if(err)
                    console.log('retrieving tags list', err);
                else
                    tagList.setTagList(tags);
            }
            );
    tagListCollapsable.setContent(tagList.element());

    // Create the photograph list.
    var photographListCollapsable = new Collapsable('Photographs', null);
    this._element(photographListCollapsable.element());

    var photographList = new PhotographList(
            function(photograph) {
                console.log('photograph id: ' + photograph.photograph_id);
            }
            );
    photographList.setList([]);
    photographListCollapsable.setContent(photographList.element());
}

