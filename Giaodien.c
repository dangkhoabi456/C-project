#include <gtk/gtk.h>

// Khai báo trước hàm xử lý sự kiện
static void onClicked(GtkWidget *widget, gpointer data);
static void onNhapBienSoXe(GtkWidget *widget, gpointer data);

// Hàm callback được gọi khi ứng dụng khởi động
static void activate(GtkApplication *app, gpointer user_data) {
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button_box;
    GtkWidget *label;
    GtkWidget *containerBox;

    // Tạo cửa sổ chính
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Quản lý bãi giữ xe");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);

    // Tạo hộp chứa theo chiều dọc
    containerBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);

    // ==== TẠO VÀ THÊM 3 NÚT GÓC TRÊN PHẢI ====
    GtkWidget *top_right_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);

    GtkWidget *btn1 = gtk_button_new_with_label("Thêm");
    GtkWidget *btn2 = gtk_button_new_with_label("Xóa");
    GtkWidget *btn3 = gtk_button_new_with_label("Thanh toán");

    g_signal_connect(btn1, "clicked", G_CALLBACK(onNhapBienSoXe), window);
    g_signal_connect(btn2, "clicked", G_CALLBACK(onClicked), NULL);
    g_signal_connect(btn3, "clicked", G_CALLBACK(onClicked), NULL);

    gtk_box_pack_start(GTK_BOX(top_right_box), btn1, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_right_box), btn2, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(top_right_box), btn3, FALSE, FALSE, 0);

    gtk_widget_set_halign(top_right_box, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(containerBox), top_right_box, FALSE, FALSE, 5);

    // ==== LABEL ====
    label = gtk_label_new("Tổng xe vào/xe ra và Tổng doanh thu");
    gtk_box_pack_start(GTK_BOX(containerBox), label, FALSE, FALSE, 5);

    // ==== NÚT "MỞ TAB" ====
    button_box = gtk_button_box_new(GTK_ORIENTATION_HORIZONTAL);
    button = gtk_button_new_with_label("Mở tab");
    g_signal_connect(button, "clicked", G_CALLBACK(onClicked), NULL);
    gtk_container_add(GTK_CONTAINER(button_box), button);
    gtk_box_pack_start(GTK_BOX(containerBox), button_box, FALSE, FALSE, 5);

    // ==== TẠO NOTEBOOK (CÁC TAB) ====
    GtkWidget *notebook = gtk_notebook_new();

    // === TAB 1: Trang chủ ===
    GtkWidget *tab_label1 = gtk_label_new("Trang chủ");
    GtkWidget *tab_content1 = gtk_label_new("Chào mừng đến với hệ thống quản lý bãi giữ xe");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_content1, tab_label1);

    // === TAB 2: Thống kê ===
    GtkWidget *tab_label2 = gtk_label_new("Thống kê");
    GtkWidget *tab_content2 = gtk_label_new("Dữ liệu thống kê xe vào/ra, doanh thu, v.v...");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_content2, tab_label2);

    // === TAB 3: Bãi xe ===
    GtkWidget *tab_label3 = gtk_label_new("Bãi xe");
    GtkWidget *tab_content3 = gtk_label_new("Danh sách xe đang gửi tại bãi");
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab_content3, tab_label3);

    // Thêm notebook vào container chính
    gtk_box_pack_start(GTK_BOX(containerBox), notebook, TRUE, TRUE, 5);

    // ==== THÊM VÀO CỬA SỔ & HIỂN THỊ ====
    gtk_container_add(GTK_CONTAINER(window), containerBox);
    gtk_widget_show_all(window);
}

// Hàm xử lý nhập biển số
static void onNhapBienSoXe(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog;
    GtkWidget *content_area;
    GtkWidget *entry;

    GtkWindow *parent_window = GTK_WINDOW(data); // truyền window vào làm cha

    // Tạo dialog
    dialog = gtk_dialog_new_with_buttons("Thêm",
                                         parent_window,
                                         GTK_DIALOG_MODAL,
                                         "_OK", GTK_RESPONSE_OK,
                                         "_Hủy", GTK_RESPONSE_CANCEL,
                                         NULL);

    // Lấy phần hiển thị chính của dialog
    content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Tạo ô nhập
    entry = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(entry), "VD: 59A1-123.45");

    // Thêm entry vào hộp thoại
    gtk_container_add(GTK_CONTAINER(content_area), entry);

    gtk_widget_show_all(dialog);

    // Chờ người dùng phản hồi
    gint response = gtk_dialog_run(GTK_DIALOG(dialog));

    if (response == GTK_RESPONSE_OK) {
        const gchar *input = gtk_entry_get_text(GTK_ENTRY(entry));
        g_print("Biển số nhập: %s\n", input);
    }

    gtk_widget_destroy(dialog);
}

// Hàm xử lý sự kiện khi nhấn nút
static void onClicked(GtkWidget *widget, gpointer data) {
    g_print("Đã mở\n");
}

int main(int argc, char **argv) {
    GtkApplication *app;
    int status;

    // Tạo ứng dụng GTK
    app = gtk_application_new("hello.world", G_APPLICATION_FLAGS_NONE);

    // Kết nối tín hiệu "activate" với hàm activate
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

    // Chạy ứng dụng
    status = g_application_run(G_APPLICATION(app), argc, argv);

    // Giải phóng bộ nhớ
    g_object_unref(app);
    return status;
}
