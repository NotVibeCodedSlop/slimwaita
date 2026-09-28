/*
 * Copyright (C) 2021-2022 Purism SPC
 * Copyright (C) 2024 GNOME Foundation Inc.
 *Copyright (c) deepseek hallucinations
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */

#include "config.h"
#include <glib/gi18n-lib.h>

#include "adw-about-dialog.h"

/**
 * AdwAboutDialog:
 *
 * A dialog showing information about the application.
 *
 * CARPET implementation. Stores properties and emits notifications, but does
 * not build any UI, parse AppStream metadata, or parse release notes markup.
 *
 * Since: 1.5
 */

typedef struct {
  char *name;
  char **people;
} CreditsSection;

typedef struct {
  char *title;
  char *copyright;
  char *license;
  GtkLicense license_type;
} LegalSection;

struct _AdwAboutDialog {
  AdwDialog parent_instance;

  char *appdata_resource_path;
  char *application_icon;
  char *application_name;
  char *developer_name;
  char *version;
  char *release_notes_version;
  char *release_notes;
  char *comments;
  char *website;
  char *support_url;
  char *issue_url;
  char *debug_info;
  char *debug_info_filename;
  char **developers;
  char **designers;
  char **artists;
  char **documenters;
  char *translator_credits;
  GSList *credit_sections;
  char *copyright;
  char *license;
  GtkLicense license_type;
  GSList *legal_sections;
  char *other_apps_title;
};

G_DEFINE_FINAL_TYPE (AdwAboutDialog, adw_about_dialog, ADW_TYPE_DIALOG)

enum {
  PROP_0,
  PROP_APPDATA_RESOURCE_PATH,
  PROP_APPLICATION_ICON,
  PROP_APPLICATION_NAME,
  PROP_DEVELOPER_NAME,
  PROP_VERSION,
  PROP_RELEASE_NOTES_VERSION,
  PROP_RELEASE_NOTES,
  PROP_COMMENTS,
  PROP_WEBSITE,
  PROP_SUPPORT_URL,
  PROP_ISSUE_URL,
  PROP_DEBUG_INFO,
  PROP_DEBUG_INFO_FILENAME,
  PROP_DEVELOPERS,
  PROP_DESIGNERS,
  PROP_ARTISTS,
  PROP_DOCUMENTERS,
  PROP_TRANSLATOR_CREDITS,
  PROP_COPYRIGHT,
  PROP_LICENSE_TYPE,
  PROP_LICENSE,
  PROP_OTHER_APPS_TITLE,
  LAST_PROP,
};

static GParamSpec *props[LAST_PROP];

enum {
  SIGNAL_ACTIVATE_LINK,
  SIGNAL_LAST_SIGNAL,
};

static guint signals[SIGNAL_LAST_SIGNAL];

static void
free_credit_section (CreditsSection *section)
{
  g_free (section->name);
  g_strfreev (section->people);
  g_free (section);
}

static void
free_legal_section (LegalSection *section)
{
  g_free (section->title);
  g_free (section->copyright);
  g_free (section->license);
  g_free (section);
}

static void
adw_about_dialog_get_property (GObject    *object,
                               guint       prop_id,
                               GValue     *value,
                               GParamSpec *pspec)
{
  AdwAboutDialog *self = ADW_ABOUT_DIALOG (object);

  switch (prop_id) {
    case PROP_APPDATA_RESOURCE_PATH:
      g_value_set_string (value, self->appdata_resource_path);
      break;
    case PROP_APPLICATION_ICON:
      g_value_set_string (value, self->application_icon);
      break;
    case PROP_APPLICATION_NAME:
      g_value_set_string (value, self->application_name);
      break;
    case PROP_DEVELOPER_NAME:
      g_value_set_string (value, self->developer_name);
      break;
    case PROP_VERSION:
      g_value_set_string (value, self->version);
      break;
    case PROP_RELEASE_NOTES_VERSION:
      g_value_set_string (value, self->release_notes_version);
      break;
    case PROP_RELEASE_NOTES:
      g_value_set_string (value, self->release_notes);
      break;
    case PROP_COMMENTS:
      g_value_set_string (value, self->comments);
      break;
    case PROP_WEBSITE:
      g_value_set_string (value, self->website);
      break;
    case PROP_SUPPORT_URL:
      g_value_set_string (value, self->support_url);
      break;
    case PROP_ISSUE_URL:
      g_value_set_string (value, self->issue_url);
      break;
    case PROP_DEBUG_INFO:
      g_value_set_string (value, self->debug_info);
      break;
    case PROP_DEBUG_INFO_FILENAME:
      g_value_set_string (value, self->debug_info_filename);
      break;
    case PROP_DEVELOPERS:
      g_value_set_boxed (value, self->developers);
      break;
    case PROP_DESIGNERS:
      g_value_set_boxed (value, self->designers);
      break;
    case PROP_ARTISTS:
      g_value_set_boxed (value, self->artists);
      break;
    case PROP_DOCUMENTERS:
      g_value_set_boxed (value, self->documenters);
      break;
    case PROP_TRANSLATOR_CREDITS:
      g_value_set_string (value, self->translator_credits);
      break;
    case PROP_COPYRIGHT:
      g_value_set_string (value, self->copyright);
      break;
    case PROP_LICENSE_TYPE:
      g_value_set_enum (value, self->license_type);
      break;
    case PROP_LICENSE:
      g_value_set_string (value, self->license);
      break;
    case PROP_OTHER_APPS_TITLE:
      g_value_set_string (value, self->other_apps_title);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}

static void
adw_about_dialog_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  AdwAboutDialog *self = ADW_ABOUT_DIALOG (object);

  switch (prop_id) {
    case PROP_APPDATA_RESOURCE_PATH:
      g_set_str (&self->appdata_resource_path, g_value_get_string (value));
      break;
    case PROP_APPLICATION_ICON:
      adw_about_dialog_set_application_icon (self, g_value_get_string (value));
      break;
    case PROP_APPLICATION_NAME:
      adw_about_dialog_set_application_name (self, g_value_get_string (value));
      break;
    case PROP_DEVELOPER_NAME:
      adw_about_dialog_set_developer_name (self, g_value_get_string (value));
      break;
    case PROP_VERSION:
      adw_about_dialog_set_version (self, g_value_get_string (value));
      break;
    case PROP_RELEASE_NOTES_VERSION:
      adw_about_dialog_set_release_notes_version (self, g_value_get_string (value));
      break;
    case PROP_RELEASE_NOTES:
      adw_about_dialog_set_release_notes (self, g_value_get_string (value));
      break;
    case PROP_COMMENTS:
      adw_about_dialog_set_comments (self, g_value_get_string (value));
      break;
    case PROP_WEBSITE:
      adw_about_dialog_set_website (self, g_value_get_string (value));
      break;
    case PROP_SUPPORT_URL:
      adw_about_dialog_set_support_url (self, g_value_get_string (value));
      break;
    case PROP_ISSUE_URL:
      adw_about_dialog_set_issue_url (self, g_value_get_string (value));
      break;
    case PROP_DEBUG_INFO:
      adw_about_dialog_set_debug_info (self, g_value_get_string (value));
      break;
    case PROP_DEBUG_INFO_FILENAME:
      adw_about_dialog_set_debug_info_filename (self, g_value_get_string (value));
      break;
    case PROP_DEVELOPERS:
      adw_about_dialog_set_developers (self, g_value_get_boxed (value));
      break;
    case PROP_DESIGNERS:
      adw_about_dialog_set_designers (self, g_value_get_boxed (value));
      break;
    case PROP_ARTISTS:
      adw_about_dialog_set_artists (self, g_value_get_boxed (value));
      break;
    case PROP_DOCUMENTERS:
      adw_about_dialog_set_documenters (self, g_value_get_boxed (value));
      break;
    case PROP_TRANSLATOR_CREDITS:
      adw_about_dialog_set_translator_credits (self, g_value_get_string (value));
      break;
    case PROP_COPYRIGHT:
      adw_about_dialog_set_copyright (self, g_value_get_string (value));
      break;
    case PROP_LICENSE_TYPE:
      adw_about_dialog_set_license_type (self, g_value_get_enum (value));
      break;
    case PROP_LICENSE:
      adw_about_dialog_set_license (self, g_value_get_string (value));
      break;
    case PROP_OTHER_APPS_TITLE:
      adw_about_dialog_set_other_apps_title (self, g_value_get_string (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}

static void
adw_about_dialog_finalize (GObject *object)
{
  AdwAboutDialog *self = ADW_ABOUT_DIALOG (object);

  g_free (self->appdata_resource_path);
  g_free (self->application_icon);
  g_free (self->application_name);
  g_free (self->developer_name);
  g_free (self->version);
  g_free (self->release_notes_version);
  g_free (self->release_notes);
  g_free (self->comments);
  g_free (self->website);
  g_free (self->support_url);
  g_free (self->issue_url);
  g_free (self->debug_info);
  g_free (self->debug_info_filename);

  g_strfreev (self->developers);
  g_strfreev (self->designers);
  g_strfreev (self->artists);
  g_strfreev (self->documenters);
  g_free (self->translator_credits);
  g_slist_free_full (self->credit_sections, (GDestroyNotify) free_credit_section);

  g_free (self->copyright);
  g_free (self->license);
  g_free (self->other_apps_title);
  g_slist_free_full (self->legal_sections, (GDestroyNotify) free_legal_section);

  G_OBJECT_CLASS (adw_about_dialog_parent_class)->finalize (object);
}

static void
adw_about_dialog_class_init (AdwAboutDialogClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = adw_about_dialog_get_property;
  object_class->set_property = adw_about_dialog_set_property;
  object_class->finalize = adw_about_dialog_finalize;

  props[PROP_APPDATA_RESOURCE_PATH] =
  g_param_spec_string ("appdata-resource-path", NULL, NULL, NULL,
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT_ONLY | G_PARAM_STATIC_STRINGS);
  props[PROP_APPLICATION_ICON] =
  g_param_spec_string ("application-icon", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_APPLICATION_NAME] =
  g_param_spec_string ("application-name", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_DEVELOPER_NAME] =
  g_param_spec_string ("developer-name", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_VERSION] =
  g_param_spec_string ("version", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_RELEASE_NOTES_VERSION] =
  g_param_spec_string ("release-notes-version", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_CONSTRUCT | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_RELEASE_NOTES] =
  g_param_spec_string ("release-notes", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_COMMENTS] =
  g_param_spec_string ("comments", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_WEBSITE] =
  g_param_spec_string ("website", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_SUPPORT_URL] =
  g_param_spec_string ("support-url", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_ISSUE_URL] =
  g_param_spec_string ("issue-url", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_DEBUG_INFO] =
  g_param_spec_string ("debug-info", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_DEBUG_INFO_FILENAME] =
  g_param_spec_string ("debug-info-filename", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_DEVELOPERS] =
  g_param_spec_boxed ("developers", NULL, NULL, G_TYPE_STRV,
                      G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  props[PROP_DESIGNERS] =
  g_param_spec_boxed ("designers", NULL, NULL, G_TYPE_STRV,
                      G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  props[PROP_ARTISTS] =
  g_param_spec_boxed ("artists", NULL, NULL, G_TYPE_STRV,
                      G_PARAM_READWRITE | G_PARAM_EXPLICIT_NOTIFY | G_PARAM_STATIC_STRINGS);
  props[PROP_DOCUMENTERS] =
  g_param_spec_boxed ("documenters", NULL, NULL, G_TYPE_STRV,
                      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_TRANSLATOR_CREDITS] =
  g_param_spec_string ("translator-credits", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_COPYRIGHT] =
  g_param_spec_string ("copyright", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_LICENSE_TYPE] =
  g_param_spec_enum ("license-type", NULL, NULL, GTK_TYPE_LICENSE,
                     GTK_LICENSE_UNKNOWN,
                     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_LICENSE] =
  g_param_spec_string ("license", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);
  props[PROP_OTHER_APPS_TITLE] =
  g_param_spec_string ("other-apps-title", NULL, NULL, "",
                       G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_EXPLICIT_NOTIFY);

  g_object_class_install_properties (object_class, LAST_PROP, props);

  signals[SIGNAL_ACTIVATE_LINK] =
  g_signal_new ("activate-link",
                G_TYPE_FROM_CLASS (klass),
                G_SIGNAL_RUN_LAST,
                0,
                g_signal_accumulator_true_handled,
                NULL,
                NULL,
                G_TYPE_BOOLEAN,
                1,
                G_TYPE_STRING);
}

static void
adw_about_dialog_init (AdwAboutDialog *self)
{
  self->application_icon = g_strdup ("");
  self->application_name = g_strdup ("");
  self->developer_name = g_strdup ("");
  self->version = g_strdup ("");
  self->release_notes_version = g_strdup ("");
  self->release_notes = g_strdup ("");
  self->comments = g_strdup ("");
  self->website = g_strdup ("");
  self->support_url = g_strdup ("");
  self->issue_url = g_strdup ("");
  self->debug_info = g_strdup ("");
  self->debug_info_filename = g_strdup ("");
  self->copyright = g_strdup ("");
  self->license = g_strdup ("");
  self->translator_credits = g_strdup ("");
  self->other_apps_title = g_strdup ("");
  self->license_type = GTK_LICENSE_UNKNOWN;
}

/* Constructors */

AdwDialog *
adw_about_dialog_new (void)
{
  return g_object_new (ADW_TYPE_ABOUT_DIALOG, NULL);
}

AdwDialog *
adw_about_dialog_new_from_appdata (const char *resource_path,
                                   const char *release_notes_version)
{
  g_return_val_if_fail (resource_path != NULL, NULL);

  return g_object_new (ADW_TYPE_ABOUT_DIALOG,
                       "appdata-resource-path", resource_path,
                       "release-notes-version", release_notes_version ? release_notes_version : "",
                       NULL);
}

/* Simple string properties */

#define DEFINE_STRING_PROPERTY(Name, name, field)                              \
const char *                                                                 \
adw_about_dialog_get_##name (AdwAboutDialog *self)                           \
{                                                                            \
  g_return_val_if_fail (ADW_IS_ABOUT_DIALOG (self), NULL);                   \
  return self->field;                                                        \
}                                                                            \
\
void                                                                         \
adw_about_dialog_set_##name (AdwAboutDialog *self, const char *value)        \
{                                                                            \
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));                             \
  g_return_if_fail (value != NULL);                                          \
  \
  if (!g_set_str (&self->field, value))                                      \
    return;                                                                  \
    \
    g_object_notify_by_pspec (G_OBJECT (self), props[PROP_##Name]);            \
}

DEFINE_STRING_PROPERTY (APPLICATION_ICON, application_icon, application_icon)
DEFINE_STRING_PROPERTY (APPLICATION_NAME, application_name, application_name)
DEFINE_STRING_PROPERTY (DEVELOPER_NAME, developer_name, developer_name)
DEFINE_STRING_PROPERTY (VERSION, version, version)
DEFINE_STRING_PROPERTY (RELEASE_NOTES_VERSION, release_notes_version, release_notes_version)
DEFINE_STRING_PROPERTY (RELEASE_NOTES, release_notes, release_notes)
DEFINE_STRING_PROPERTY (COMMENTS, comments, comments)
DEFINE_STRING_PROPERTY (WEBSITE, website, website)
DEFINE_STRING_PROPERTY (SUPPORT_URL, support_url, support_url)
DEFINE_STRING_PROPERTY (ISSUE_URL, issue_url, issue_url)
DEFINE_STRING_PROPERTY (DEBUG_INFO, debug_info, debug_info)
DEFINE_STRING_PROPERTY (DEBUG_INFO_FILENAME, debug_info_filename, debug_info_filename)
DEFINE_STRING_PROPERTY (TRANSLATOR_CREDITS, translator_credits, translator_credits)
DEFINE_STRING_PROPERTY (COPYRIGHT, copyright, copyright)
DEFINE_STRING_PROPERTY (OTHER_APPS_TITLE, other_apps_title, other_apps_title)

const char *
adw_about_dialog_get_appdata_resource_path (AdwAboutDialog *self)
{
  g_return_val_if_fail (ADW_IS_ABOUT_DIALOG (self), NULL);
  return self->appdata_resource_path;
}

/* String lists */

#define DEFINE_STRV_PROPERTY(Name, name, field)                                \
const char * const *                                                         \
adw_about_dialog_get_##name (AdwAboutDialog *self)                           \
{                                                                            \
  g_return_val_if_fail (ADW_IS_ABOUT_DIALOG (self), NULL);                   \
  return (const char * const *) self->field;                                 \
}                                                                            \
\
void                                                                         \
adw_about_dialog_set_##name (AdwAboutDialog *self, const char **value)       \
{                                                                            \
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));                             \
  \
  if ((const char **) self->field == value)                                  \
    return;                                                                  \
    \
    g_strfreev (self->field);                                                  \
    self->field = g_strdupv ((char **) value);                                 \
    \
    g_object_notify_by_pspec (G_OBJECT (self), props[PROP_##Name]);            \
}

DEFINE_STRV_PROPERTY (DEVELOPERS, developers, developers)
DEFINE_STRV_PROPERTY (DESIGNERS, designers, designers)
DEFINE_STRV_PROPERTY (ARTISTS, artists, artists)
DEFINE_STRV_PROPERTY (DOCUMENTERS, documenters, documenters)

/* License */

GtkLicense
adw_about_dialog_get_license_type (AdwAboutDialog *self)
{
  g_return_val_if_fail (ADW_IS_ABOUT_DIALOG (self), GTK_LICENSE_UNKNOWN);
  return self->license_type;
}

void
adw_about_dialog_set_license_type (AdwAboutDialog *self,
                                   GtkLicense      license_type)
{
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (license_type >= GTK_LICENSE_UNKNOWN);

  if (self->license_type == license_type)
    return;

  if (license_type != GTK_LICENSE_CUSTOM)
    g_set_str (&self->license, "");

  self->license_type = license_type;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_LICENSE]);
  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_LICENSE_TYPE]);
}

const char *
adw_about_dialog_get_license (AdwAboutDialog *self)
{
  g_return_val_if_fail (ADW_IS_ABOUT_DIALOG (self), NULL);
  return self->license;
}

void
adw_about_dialog_set_license (AdwAboutDialog *self,
                              const char     *license)
{
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (license != NULL);

  if (g_strcmp0 (self->license, license) == 0)
    return;

  g_object_freeze_notify (G_OBJECT (self));

  g_set_str (&self->license, license);
  self->license_type = GTK_LICENSE_CUSTOM;

  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_LICENSE]);
  g_object_notify_by_pspec (G_OBJECT (self), props[PROP_LICENSE_TYPE]);

  g_object_thaw_notify (G_OBJECT (self));
}

/* Links / other apps — no-op in the stub */

void
adw_about_dialog_add_link (AdwAboutDialog *self,
                           const char     *title,
                           const char     *url)
{
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (title != NULL);
  g_return_if_fail (url != NULL);
}

void
adw_about_dialog_add_other_app (AdwAboutDialog *self,
                                const char     *appid,
                                const char     *name,
                                const char     *summary)
{
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (appid != NULL);
  g_return_if_fail (name != NULL);
  g_return_if_fail (summary != NULL);
}

/* Credits / acknowledgements / legal sections */

void
adw_about_dialog_add_credit_section (AdwAboutDialog  *self,
                                     const char      *name,
                                     const char     **people)
{
  CreditsSection *section;

  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (people != NULL);

  section = g_new0 (CreditsSection, 1);
  section->name = g_strdup (name);
  section->people = g_strdupv ((char **) people);

  self->credit_sections = g_slist_append (self->credit_sections, section);
}

void
adw_about_dialog_add_acknowledgement_section (AdwAboutDialog  *self,
                                              const char      *name,
                                              const char     **people)
{
  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (people != NULL);
}

void
adw_about_dialog_add_legal_section (AdwAboutDialog *self,
                                    const char     *title,
                                    const char     *copyright,
                                    GtkLicense      license_type,
                                    const char     *license)
{
  LegalSection *section;

  g_return_if_fail (ADW_IS_ABOUT_DIALOG (self));
  g_return_if_fail (title != NULL);
  g_return_if_fail (license_type >= GTK_LICENSE_UNKNOWN);

  section = g_new0 (LegalSection, 1);
  section->title = g_strdup (title);
  section->copyright = g_strdup (copyright);
  section->license_type = license_type;
  section->license = g_strdup (license);

  self->legal_sections = g_slist_append (self->legal_sections, section);
}

/* Convenience constructors */

void
adw_show_about_dialog (GtkWidget  *parent,
                       const char *first_property_name,
                       ...)
{
  AdwDialog *dialog;
  va_list var_args;

  g_return_if_fail (GTK_IS_WIDGET (parent));

  dialog = adw_about_dialog_new ();

  va_start (var_args, first_property_name);
  g_object_set_valist (G_OBJECT (dialog), first_property_name, var_args);
  va_end (var_args);

  adw_dialog_present (dialog, parent);
}

void
adw_show_about_dialog_from_appdata (GtkWidget  *parent,
                                    const char *resource_path,
                                    const char *release_notes_version,
                                    const char *first_property_name,
                                    ...)
{
  AdwDialog *dialog;
  va_list var_args;

  g_return_if_fail (GTK_IS_WIDGET (parent));

  dialog = adw_about_dialog_new_from_appdata (resource_path,
                                              release_notes_version);

  va_start (var_args, first_property_name);
  g_object_set_valist (G_OBJECT (dialog), first_property_name, var_args);
  va_end (var_args);

  adw_dialog_present (dialog, parent);
}
