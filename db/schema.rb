# encoding: UTF-8
# This file is auto-generated from the current state of the database. Instead
# of editing this file, please use the migrations feature of Active Record to
# incrementally modify your database, and then regenerate this schema definition.
#
# Note that this schema.rb definition is the authoritative source for your
# database schema. If you need to create the application database on another
# system, you should be using db:schema:load, not running all the migrations
# from scratch. The latter is a flawed and unsustainable approach (the more migrations
# you'll amass, the slower it'll run and the greater likelihood for issues).
#
# It's strongly recommended that you check this file into your version control system.

ActiveRecord::Schema.define(version: 20160209090104) do

  # These are extensions that must be enabled in order to support this database
  enable_extension "plpgsql"
  enable_extension "pg_trgm"

  create_table "active_admin_comments", force: :cascade do |t|
    t.string   "namespace"
    t.text     "body"
    t.string   "resource_id",   null: false
    t.string   "resource_type", null: false
    t.integer  "author_id"
    t.string   "author_type"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "active_admin_comments", ["author_type", "author_id"], name: "index_active_admin_comments_on_author_type_and_author_id", using: :btree
  add_index "active_admin_comments", ["namespace"], name: "index_active_admin_comments_on_namespace", using: :btree
  add_index "active_admin_comments", ["resource_type", "resource_id"], name: "index_active_admin_comments_on_resource_type_and_resource_id", using: :btree

  create_table "admin_users", force: :cascade do |t|
    t.string   "email",                  default: "", null: false
    t.string   "encrypted_password",     default: "", null: false
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",          default: 0,  null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "username"
    t.string   "avatar"
    t.string   "fullname"
    t.string   "admin_type"
  end

  add_index "admin_users", ["email"], name: "index_admin_users_on_email", unique: true, using: :btree
  add_index "admin_users", ["reset_password_token"], name: "index_admin_users_on_reset_password_token", unique: true, using: :btree

  create_table "batches", force: :cascade do |t|
    t.string   "name"
    t.text     "description"
    t.date     "start_date"
    t.date     "end_date"
    t.datetime "created_at",   null: false
    t.datetime "updated_at",   null: false
    t.integer  "batch_number"
  end

  create_table "connect_requests", force: :cascade do |t|
    t.integer  "connect_slot_id"
    t.integer  "startup_id"
    t.text     "questions"
    t.string   "status"
    t.string   "meeting_link"
    t.datetime "created_at",             null: false
    t.datetime "updated_at",             null: false
    t.datetime "confirmed_at"
    t.datetime "feedback_mails_sent_at"
    t.integer  "rating_of_faculty"
    t.integer  "rating_of_team"
  end

  add_index "connect_requests", ["connect_slot_id"], name: "index_connect_requests_on_connect_slot_id", using: :btree
  add_index "connect_requests", ["startup_id"], name: "index_connect_requests_on_startup_id", using: :btree

  create_table "connect_slots", force: :cascade do |t|
    t.integer  "faculty_id"
    t.datetime "slot_at"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
  end

  add_index "connect_slots", ["faculty_id"], name: "index_connect_slots_on_faculty_id", using: :btree

  create_table "delayed_jobs", force: :cascade do |t|
    t.integer  "priority",   default: 0, null: false
    t.integer  "attempts",   default: 0, null: false
    t.text     "handler",                null: false
    t.text     "last_error"
    t.datetime "run_at"
    t.datetime "locked_at"
    t.datetime "failed_at"
    t.string   "locked_by"
    t.string   "queue"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  add_index "delayed_jobs", ["priority", "run_at"], name: "delayed_jobs_priority", using: :btree

  create_table "faculty", force: :cascade do |t|
    t.string   "name"
    t.string   "title"
    t.string   "key_skills"
    t.string   "linkedin_url"
    t.string   "category"
    t.string   "image"
    t.integer  "sort_index"
    t.datetime "created_at",         null: false
    t.datetime "updated_at",         null: false
    t.string   "email"
    t.string   "token"
    t.boolean  "self_service"
    t.string   "current_commitment"
  end

  add_index "faculty", ["category"], name: "index_faculty_on_category", using: :btree

  create_table "features", force: :cascade do |t|
    t.string   "key"
    t.string   "value"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "karma_points", force: :cascade do |t|
    t.integer  "user_id"
    t.integer  "points"
    t.string   "activity_type"
    t.datetime "created_at",    null: false
    t.datetime "updated_at",    null: false
    t.integer  "source_id"
    t.string   "source_type"
    t.integer  "startup_id"
  end

  add_index "karma_points", ["source_id"], name: "index_karma_points_on_source_id", using: :btree
  add_index "karma_points", ["startup_id"], name: "index_karma_points_on_startup_id", using: :btree
  add_index "karma_points", ["user_id"], name: "index_karma_points_on_user_id", using: :btree

  create_table "public_slack_messages", force: :cascade do |t|
    t.text     "body"
    t.string   "slack_username"
    t.integer  "user_id"
    t.string   "channel"
    t.datetime "created_at",     null: false
    t.datetime "updated_at",     null: false
  end

  add_index "public_slack_messages", ["user_id"], name: "index_public_slack_messages_on_user_id", using: :btree

  create_table "resources", force: :cascade do |t|
    t.string   "file"
    t.string   "thumbnail"
    t.string   "title"
    t.text     "description"
    t.datetime "created_at",               null: false
    t.datetime "updated_at",               null: false
    t.string   "share_status"
    t.integer  "downloads",    default: 0
    t.string   "slug"
    t.integer  "batch_id"
  end

  add_index "resources", ["batch_id"], name: "index_resources_on_batch_id", using: :btree
  add_index "resources", ["share_status", "batch_id"], name: "index_resources_on_share_status_and_batch_id", using: :btree
  add_index "resources", ["slug"], name: "index_resources_on_slug", using: :btree

  create_table "startup_categories", force: :cascade do |t|
    t.string   "name"
    t.datetime "created_at"
    t.datetime "updated_at"
  end

  create_table "startup_categories_startups", id: false, force: :cascade do |t|
    t.integer "startup_id"
    t.integer "startup_category_id"
  end

  create_table "startup_feedback", force: :cascade do |t|
    t.text     "feedback"
    t.string   "reference_url"
    t.datetime "created_at",    null: false
    t.datetime "updated_at",    null: false
    t.integer  "startup_id"
    t.datetime "sent_at"
    t.integer  "faculty_id"
    t.string   "activity_type"
  end

  add_index "startup_feedback", ["faculty_id"], name: "index_startup_feedback_on_faculty_id", using: :btree

  create_table "startups", force: :cascade do |t|
    t.string   "name"
    t.string   "logo"
    t.string   "pitch"
    t.string   "website"
    t.string   "email"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "facebook_link"
    t.string   "twitter_link"
    t.text     "address"
    t.string   "registration_type"
    t.string   "approval_status",           default: "unready"
    t.string   "product_name"
    t.text     "product_description"
    t.string   "cool_fact"
    t.string   "state"
    t.string   "district"
    t.string   "product_progress"
    t.string   "presentation_link"
    t.integer  "revenue_generated"
    t.integer  "team_size"
    t.integer  "women_employees"
    t.string   "incubation_location"
    t.boolean  "agreement_sent",            default: false
    t.string   "pin"
    t.datetime "agreement_first_signed_at"
    t.datetime "agreement_last_signed_at"
    t.datetime "agreement_ends_at"
    t.boolean  "physical_incubatee"
    t.text     "metadata"
    t.string   "slug"
    t.boolean  "featured"
    t.string   "stage"
    t.string   "legal_registered_name"
    t.string   "wireframe_link"
    t.string   "prototype_link"
    t.string   "product_video"
    t.integer  "batch_id"
  end

  add_index "startups", ["batch_id"], name: "index_startups_on_batch_id", using: :btree
  add_index "startups", ["slug"], name: "index_startups_on_slug", unique: true, using: :btree
  add_index "startups", ["stage"], name: "index_startups_on_stage", using: :btree

  create_table "target_templates", force: :cascade do |t|
    t.integer  "days_from_start"
    t.string   "role"
    t.string   "title"
    t.text     "description"
    t.string   "completion_instructions"
    t.string   "resource_url"
    t.datetime "created_at",              null: false
    t.datetime "updated_at",              null: false
    t.text     "slideshow_embed"
  end

  create_table "targets", force: :cascade do |t|
    t.string   "role"
    t.integer  "startup_id"
    t.integer  "assigner_id"
    t.string   "title"
    t.text     "description"
    t.string   "resource_url"
    t.datetime "created_at",              null: false
    t.datetime "updated_at",              null: false
    t.string   "status"
    t.string   "completion_instructions"
    t.datetime "due_date"
    t.datetime "completed_at"
    t.text     "completion_comment"
    t.text     "slideshow_embed"
  end

  add_index "targets", ["assigner_id"], name: "index_targets_on_assigner_id", using: :btree
  add_index "targets", ["startup_id"], name: "index_targets_on_startup_id", using: :btree

  create_table "team_members", force: :cascade do |t|
    t.string   "name"
    t.string   "email"
    t.string   "roles"
    t.string   "avatar"
    t.integer  "startup_id"
    t.datetime "created_at",                        null: false
    t.datetime "updated_at",                        null: false
    t.boolean  "avatar_processing", default: false
  end

  add_index "team_members", ["startup_id"], name: "index_team_members_on_startup_id", using: :btree

  create_table "timeline_event_types", force: :cascade do |t|
    t.string   "key"
    t.string   "title"
    t.text     "sample_text"
    t.datetime "created_at",      null: false
    t.datetime "updated_at",      null: false
    t.string   "badge"
    t.string   "role"
    t.string   "proof_required"
    t.string   "suggested_stage"
    t.boolean  "private"
    t.boolean  "major"
  end

  add_index "timeline_event_types", ["role"], name: "index_timeline_event_types_on_role", using: :btree

  create_table "timeline_events", force: :cascade do |t|
    t.text     "description"
    t.string   "image"
    t.integer  "startup_id"
    t.text     "links"
    t.datetime "created_at",             null: false
    t.datetime "updated_at",             null: false
    t.date     "event_on"
    t.datetime "verified_at"
    t.integer  "timeline_event_type_id"
    t.string   "verified_status"
    t.string   "grade"
    t.integer  "target_id"
    t.integer  "user_id"
  end

  add_index "timeline_events", ["startup_id"], name: "index_timeline_events_on_startup_id", using: :btree
  add_index "timeline_events", ["target_id"], name: "index_timeline_events_on_target_id", using: :btree
  add_index "timeline_events", ["timeline_event_type_id"], name: "index_timeline_events_on_timeline_event_type_id", using: :btree
  add_index "timeline_events", ["user_id"], name: "index_timeline_events_on_user_id", using: :btree

  create_table "universities", force: :cascade do |t|
    t.string   "name"
    t.datetime "created_at", null: false
    t.datetime "updated_at", null: false
    t.string   "location"
  end

  create_table "users", force: :cascade do |t|
    t.string   "email"
    t.string   "first_name"
    t.datetime "created_at"
    t.datetime "updated_at"
    t.string   "avatar"
    t.string   "encrypted_password",        default: ""
    t.string   "reset_password_token"
    t.datetime "reset_password_sent_at"
    t.datetime "remember_created_at"
    t.integer  "sign_in_count",             default: 0,     null: false
    t.datetime "current_sign_in_at"
    t.datetime "last_sign_in_at"
    t.string   "current_sign_in_ip"
    t.string   "last_sign_in_ip"
    t.string   "confirmation_token"
    t.datetime "confirmed_at"
    t.datetime "confirmation_sent_at"
    t.string   "unconfirmed_email"
    t.string   "invitation_token"
    t.datetime "invitation_created_at"
    t.datetime "invitation_sent_at"
    t.datetime "invitation_accepted_at"
    t.integer  "invitation_limit"
    t.integer  "invited_by_id"
    t.string   "invited_by_type"
    t.integer  "startup_id"
    t.string   "title"
    t.string   "linkedin_url"
    t.string   "twitter_url"
    t.date     "born_on"
    t.string   "auth_token"
    t.boolean  "is_founder"
    t.string   "course"
    t.string   "semester"
    t.string   "gender"
    t.string   "phone"
    t.text     "communication_address"
    t.string   "phone_verification_code"
    t.boolean  "startup_admin"
    t.integer  "year_of_graduation"
    t.string   "roll_number"
    t.string   "slack_username"
    t.integer  "university_id"
    t.string   "unconfirmed_phone"
    t.string   "roles"
    t.string   "last_name",                 default: ""
    t.string   "college_identification"
    t.boolean  "avatar_processing",         default: false
    t.string   "slack_user_id"
    t.string   "personal_website_url"
    t.string   "blog_url"
    t.string   "facebook_url"
    t.string   "angel_co_url"
    t.string   "github_url"
    t.string   "behance_url"
    t.string   "resume_url"
    t.string   "slug"
    t.string   "about"
    t.datetime "verification_code_sent_at"
  end

  add_index "users", ["confirmation_token"], name: "index_users_on_confirmation_token", unique: true, using: :btree
  add_index "users", ["invitation_token"], name: "index_users_on_invitation_token", unique: true, using: :btree
  add_index "users", ["invited_by_id"], name: "index_users_on_invited_by_id", using: :btree
  add_index "users", ["reset_password_token"], name: "index_users_on_reset_password_token", unique: true, using: :btree
  add_index "users", ["slug"], name: "index_users_on_slug", unique: true, using: :btree
  add_index "users", ["university_id"], name: "index_users_on_university_id", using: :btree

  add_foreign_key "connect_requests", "connect_slots"
  add_foreign_key "connect_requests", "startups"
  add_foreign_key "connect_slots", "faculty"
  add_foreign_key "resources", "batches"
  add_foreign_key "startup_feedback", "faculty"
  add_foreign_key "team_members", "startups"
  add_foreign_key "timeline_events", "startups"
end
